#include "../../include/core/RegistersLinearScan.hpp"

RegistersLinearScan::RegistersLinearScan() {}

RegistersLinearScan::RegistersLinearScan(std::shared_ptr<Hardware> hardware) : hardware(hardware) {}

void RegistersLinearScan::addVariableUsageInfo(const std::string &variable, uint64_t position, UsageType usageType) {
  if (variablesUsage.find(variable) == variablesUsage.end()) {
	variablesUsage[variable] = std::vector<Usage>();
	variableMemoryPosition[variable] = hardware->getFreeMemoryAddress();
  }
  auto sameUsage =
	  std::find_if(variablesUsage[variable].begin(), variablesUsage[variable].end(),
				   [position](const Usage &u) { return u.position == position; });

  if (sameUsage != variablesUsage[variable].end()) {
	if (sameUsage->type != usageType) {
	  sameUsage->type = UsageType::BOTH;
	}
  } else {
	variablesUsage[variable].emplace_back(Usage{position, usageType});
  }
}

void RegistersLinearScan::createRanges(ControlFlowGraph &controlFlowGraph) {
  flowGraph = controlFlowGraph;
  liveRanges = flowGraph.calculateVariablesLiveRanges();

  for (auto &liveRangeForVariable : liveRanges) {
	const auto variable = liveRangeForVariable.first;
	if (Hardware::isRegName(variable)) {
	  continue;
	}
	for (auto &liveRange : liveRangeForVariable.second) {
	  if (liveRange.empty()) {
		continue;
	  }
	  Range range{variable, liveRange.front(), liveRange.back(), {}};
	  range.usages = getVariableUsage(variable, range);
	  if (range.usages.empty()) {
		continue;
	  }
	  range.start = range.usages.front().position;
	  range.end = range.usages.back().position;

	  ranges.push_back(range);
	}
  }

  std::sort(ranges.begin(), ranges.end(), [](const Range &a, const Range &b) { return a.start < b.start; });
}

void RegistersLinearScan::allocateRegisters() {
  registersUsage =
	  std::map < HardwareRegister, std::vector
	  < Range >> {{RB, {}}, {RC, {}}, {RD, {}}, {RE, {}}, {RF, {}}, {RG, {}}, {RH, {}}};

  std::sort(ranges.begin(), ranges.end(), [](const Range &a, const Range &b) { return a.start < b.start; });

  for (auto &range : ranges) {
	const auto freeReg = getFreeRegister(range);
	if (freeReg.has_value()) {
	  const auto freeRegName = Hardware::registerMap[freeReg.value()].name;
	  for (const auto &usage : range.usages) {
		variableRegisters[range.owner][usage.position] = freeRegName;
	  }
	} else {
	  const auto foundRegister = splitRanges(range);
	  const auto freeRegName = Hardware::registerMap[foundRegister].name;
	  for (const auto &usage : range.usages) {
		variableRegisters[range.owner][usage.position] = freeRegName;
	  }
	}
  }

  std::vector<std::string> vars;
  for (const auto &iter : variablesUsage) {
	vars.push_back(iter.first);
  }

  std::vector<std::vector<Range>> finalRanges;
  for (const auto &var : vars) {
	finalRanges.push_back(findRangesWithVariable(var));
  }

  std::sort(finalRanges.begin(), finalRanges.end(),
			[this](const std::vector<Range> &a, const std::vector<Range> &b) {
			  size_t aActions = 0;
			  size_t bActions = 0;

			  for (const auto &range : a) {
				if (this->loadIsRequired(range)) {
				  aActions++;
				}
				if (this->storeIsRequired(range)) {
				  aActions++;
				}
			  }

			  for (const auto &range : b) {
				if (this->loadIsRequired(range)) {
				  bActions++;
				}
				if (this->storeIsRequired(range)) {
				  bActions++;
				}
			  }

			  return aActions > bActions;
			});

  for (const auto &finalRange : finalRanges) {
	if (finalRange.size() < 3) {
	  continue;
	}

	Range merged;
	for (const auto &fr : finalRange) {
	  merged.usages.insert(merged.usages.end(), fr.usages.begin(), fr.usages.end());
	}
	std::sort(merged.usages.begin(), merged.usages.end(), [](const auto &a, const auto &b) {
	  return a.position < b.position;
	});

	merged.owner = finalRange.front().owner;
	merged.start = merged.usages.front().position;
	merged.end = merged.usages.back().position;

	const auto freeRegisterForRange = getFreeRegister(merged);
	if (freeRegisterForRange.has_value()) {
	  const auto freeRegName = Hardware::registerMap[freeRegisterForRange.value()].name;
	  for (const auto &usage : merged.usages) {
		variableRegisters[merged.owner][usage.position] = freeRegName;
	  }

	  for (auto &reg : registersUsage) {
		std::vector<Range> newRanges;
		for (const auto &range : reg.second) {
		  if (range.start == merged.start && range.end == merged.end && range.owner == merged.owner) {
			newRanges.push_back(range);
		  } else if (range.owner != merged.owner) {
			newRanges.push_back(range);
		  }
		}
		std::sort(newRanges.begin(), newRanges.end(), [](const Range &a, const Range &b) { return a.start < b.start; });
		reg.second = newRanges;
	  }
	}
  }

  memoryLoad.clear();
  memoryStore.clear();

  for (const auto &reg : registersUsage) {
	for (const auto &range : reg.second) {
	  const auto variableName = range.owner;
	  const auto registerName = getVariableRegister(variableName, range.start);
	  if (loadIsRequired(range)) {
		memoryLoad[range.usages.front().position].push_back(std::make_pair(registerName,
																		   variableMemoryPosition[variableName]));
	  }

	  if (storeIsRequired(range)) {
		memoryStore[range.usages.back().position].push_back(std::make_pair(registerName,
																		   variableMemoryPosition[variableName]));
	  }
	}
  }

  std::map < uint64_t, std::vector < std::pair < std::string, uint64_t>>>
  uniqueMemoryStore;
  for (const auto &store : memoryStore) {
	const auto position = store.first;
	for (const auto &storeInfo : store.second) {
	  const auto found = std::find_if(uniqueMemoryStore[position].begin(), uniqueMemoryStore[position].end(),
									  [storeInfo](const auto &info) {
										return info.first == storeInfo.first && info.second == storeInfo.second;
									  });
	  if (found == uniqueMemoryStore[position].end()) {
		uniqueMemoryStore[position].push_back(storeInfo);
	  }
	}
  }
  memoryStore = uniqueMemoryStore;

  std::map < uint64_t, std::vector < std::pair < std::string, uint64_t>>>
  uniqueMemoryLoad;
  for (const auto &load : memoryLoad) {
	const auto position = load.first;
	for (const auto &loadInfo : load.second) {
	  const auto found = std::find_if(uniqueMemoryLoad[position].begin(), uniqueMemoryLoad[position].end(),
									  [loadInfo](const auto &info) {
										return info.first == loadInfo.first && info.second == loadInfo.second;
									  });
	  if (found == uniqueMemoryLoad[position].end()) {
		uniqueMemoryLoad[position].push_back(loadInfo);
	  }
	}
  }
  memoryLoad = uniqueMemoryLoad;
}

std::string RegistersLinearScan::getVariableRegister(const std::string &variable, uint64_t position) {
  if (variableRegisters.find(variable) == variableRegisters.end()) {
	return variable;
  }

  if (variableRegisters[variable].find(position) == variableRegisters[variable].end()) {
	return variable;
  }

  return variableRegisters[variable][position];
}

std::vector<std::pair<std::string, uint64_t>> RegistersLinearScan::getMemoryStore(uint64_t position) {
  if (memoryStore.find(position) == memoryStore.end()) {
	return {};
  }
  return memoryStore[position];
}
std::vector<std::pair<std::string, uint64_t>> RegistersLinearScan::getMemoryLoad(uint64_t position) {
  if (memoryLoad.find(position) == memoryLoad.end()) {
	return {};
  }
  return memoryLoad[position];
}

std::optional<HardwareRegister> RegistersLinearScan::getFreeRegister(const Range &newRange) {
  for (auto &reg : registersUsage) {
	const auto ranges = reg.second;

	bool anyOverlap = false;
	for (auto &range : ranges) {
	  if (rangesOverlap(range, newRange)) {
		anyOverlap = true;
		break;
	  }
	}

	if (!anyOverlap) {
	  reg.second.push_back(newRange);
	  std::sort(reg.second.begin(),
				reg.second.end(),
				[](const Range &a, const Range &b) { return a.start < b.start; });
	  return reg.first;
	}
  }

  return std::nullopt;
}

HardwareRegister RegistersLinearScan::splitRanges(const Range &newRange) {
  auto newRangeUsages = std::vector<uint64_t>();
  for (auto &usage : newRange.usages) {
	newRangeUsages.push_back(usage.position);
  }

  HardwareRegister maxRangeRegister = RA;
  size_t maxRangeIndex = 0;
  size_t maxUsageIndex = 0;
  size_t maxRangeSize = 0;
  for (const auto &reg : registersUsage) {
	for (size_t rangeIndex = 0; rangeIndex < reg.second.size(); rangeIndex++) {
	  const auto range = reg.second[rangeIndex];
	  for (size_t usageIndex = 1; usageIndex < range.usages.size(); usageIndex++) {
		if (range.usages[usageIndex - 1].position < newRange.start
			&& range.usages[usageIndex].position > newRange.end) {
		  size_t size = range.usages[usageIndex].position - range.usages[usageIndex - 1].position;
		  if (size > maxRangeSize) {
			maxRangeRegister = reg.first;
			maxRangeIndex = rangeIndex;
			maxUsageIndex = usageIndex;
			maxRangeSize = size;
		  }
		}
	  }
	}
  }

  if (maxRangeSize > 0) {
	std::vector<Range> newRanges;
	for (size_t i = 0; i < maxRangeIndex; i++) {
	  newRanges.push_back(registersUsage[maxRangeRegister][i]);
	}

	const auto dividedRanges = divideRange1(registersUsage[maxRangeRegister][maxRangeIndex], maxUsageIndex);
	newRanges.push_back(dividedRanges.first);
	newRanges.push_back(newRange);
	newRanges.push_back(dividedRanges.second);

	for (size_t i = maxRangeIndex + 1; i < registersUsage[maxRangeRegister].size(); i++) {
	  newRanges.push_back(registersUsage[maxRangeRegister][i]);
	}

	registersUsage[maxRangeRegister] = newRanges;

	return maxRangeRegister;
  }

  HardwareRegister minConflictingRegister = RA;
  size_t minConflictingUsagesNumber = std::numeric_limits<size_t>::max();
  for (const auto &reg : registersUsage) {
	std::vector<std::pair<std::string, Usage>> currentUsages;
	std::vector<uint64_t> currentUsagesPositions;
	for (const auto &range : reg.second) {
	  for (const auto &usage : range.usages) {
		if (usage.position >= newRange.start && usage.position <= newRange.end) {
		  currentUsages.push_back(std::make_pair(range.owner, usage));
		  currentUsagesPositions.push_back(usage.position);
		}
	  }
	}

	if (!vectorsIntersection(newRangeUsages, currentUsagesPositions).empty()) {
	  continue;
	}

	if (currentUsages.size() < minConflictingUsagesNumber) {
	  minConflictingRegister = reg.first;
	  minConflictingUsagesNumber = currentUsages.size();
	}
  }

  std::vector<Range> finalRanges;
  std::vector<std::pair<std::string, Usage>> minConflictingUsages;
  for (const auto &usage : newRange.usages) {
	minConflictingUsages.push_back(std::make_pair(newRange.owner, usage));
  }
  for (const auto &reg : registersUsage[minConflictingRegister]) {
	if (rangesOverlap(reg, newRange)) {
	  for (const auto &usage : reg.usages) {
		minConflictingUsages.push_back(std::make_pair(reg.owner, usage));
	  }
	} else {
	  finalRanges.push_back(reg);
	}
  }

  std::sort(minConflictingUsages.begin(), minConflictingUsages.end(), [](const auto &a, const auto &b) {
	return a.second.position < b.second.position;
  });

  uint64_t currProblemIndex = 0;
  std::string currProblemVariable = minConflictingUsages[currProblemIndex].first;
  std::vector<Usage> currProblemUsages = {minConflictingUsages[currProblemIndex].second};
  currProblemIndex++;
  while (currProblemIndex < minConflictingUsages.size()) {
	if (minConflictingUsages[currProblemIndex].first != currProblemVariable) {
	  Range nextRange{currProblemVariable, currProblemUsages.front().position, currProblemUsages.back().position,
					  currProblemUsages};
	  finalRanges.push_back(nextRange);

	  currProblemVariable = minConflictingUsages[currProblemIndex].first;
	  currProblemUsages = {minConflictingUsages[currProblemIndex].second};
	} else {
	  currProblemUsages.push_back(minConflictingUsages[currProblemIndex].second);
	}

	currProblemIndex++;
  }

  Range nextRange
	  {currProblemVariable, currProblemUsages.front().position, currProblemUsages.back().position, currProblemUsages};
  finalRanges.push_back(nextRange);

  std::sort(finalRanges.begin(), finalRanges.end(), [](const Range &a, const Range &b) { return a.start < b.start; });

  std::vector<Range> finalRangesWithoutDuplicates;
  for (const auto &range : finalRanges) {
	const auto exist =
		std::find_if(finalRangesWithoutDuplicates.begin(),
					 finalRangesWithoutDuplicates.end(),
					 [range](const Range &r) {
					   return r.start == range.start && r.end == range.end;
					 });

	if (exist == finalRangesWithoutDuplicates.end()) {
	  finalRangesWithoutDuplicates.push_back(range);
	}
  }

  registersUsage[minConflictingRegister] = finalRangesWithoutDuplicates;

  return minConflictingRegister;
}

std::vector<Usage> RegistersLinearScan::getVariableUsage(const std::string &variable,
														 const Range &range) {
  std::vector<Usage> result;
  for (auto &usage : variablesUsage[variable]) {
	if (usage.position >= range.start && usage.position <= range.end) {
	  result.push_back(usage);
	}
  }

  std::sort(result.begin(), result.end(), [](const Usage &a, const Usage &b) { return a.position < b.position; });
  return result;
}

std::vector<uint64_t> RegistersLinearScan::vectorsIntersection(const std::vector<uint64_t> &vector1,
															   const std::vector<uint64_t> &vector2) {
  std::vector<uint64_t> result;
  for (const uint64_t usage1 : vector1) {
	for (const uint64_t usage2 : vector2) {
	  if (usage1 == usage2) {
		result.push_back(usage1);
	  }
	}
  }
  return result;
}

UsageType RegistersLinearScan::getVariableUsageType(const std::string &variable, uint64_t position) {
  const auto lval = std::find_if(variablesUsage[variable].begin(), variablesUsage[variable].end(),
								 [position](const Usage &u) {
								   return u.position == position && u.type == UsageType::LVAL;
								 });
  const auto rval = std::find_if(variablesUsage[variable].begin(), variablesUsage[variable].end(),
								 [position](const Usage &u) {
								   return u.position == position && u.type == UsageType::RVAL;
								 });

  if (lval != variablesUsage[variable].end() && rval != variablesUsage[variable].end()) {
	return UsageType::BOTH;
  }
  if (lval != variablesUsage[variable].end()) {
	return UsageType::LVAL;
  }
  return UsageType::RVAL;
}

std::pair<Range, Range> RegistersLinearScan::divideRange1(const Range &range, uint64_t position) {
  std::vector<Usage> newRangeUsages1;
  for (size_t i = 0; i < position; i++) {
	newRangeUsages1.push_back(range.usages[i]);
  }
  Range newRange1{range.owner, newRangeUsages1.front().position, newRangeUsages1.back().position, newRangeUsages1};

  std::vector<Usage> newRangeUsages2;
  for (size_t i = position; i < range.usages.size(); i++) {
	newRangeUsages2.push_back(range.usages[i]);
  }
  Range newRange2{range.owner, newRangeUsages2.front().position, newRangeUsages2.back().position, newRangeUsages2};

  return std::make_pair(newRange1, newRange2);
}

std::pair<Range, Range> RegistersLinearScan::divideRange2(const Range &range, uint64_t position) {
  std::vector<Usage> newRangeUsages1;
  for (size_t i = 0; i <= position; i++) {
	newRangeUsages1.push_back(range.usages[i]);
  }
  Range newRange1{range.owner, newRangeUsages1.front().position, newRangeUsages1.back().position, newRangeUsages1};

  std::vector<Usage> newRangeUsages2;
  for (size_t i = (position + 1); i < range.usages.size(); i++) {
	newRangeUsages2.push_back(range.usages[i]);
  }
  Range newRange2{range.owner, newRangeUsages2.front().position, newRangeUsages2.back().position, newRangeUsages2};

  return std::make_pair(newRange1, newRange2);
}

std::pair<Range, Range> RegistersLinearScan::divideRange3(const Range &range, uint64_t position) {
  std::vector<Usage> newRangeUsages1;
  for (size_t i = 0; i < position; i++) {
	newRangeUsages1.push_back(range.usages[i]);
  }
  Range newRange1{range.owner, newRangeUsages1.front().position, newRangeUsages1.back().position, newRangeUsages1};

  std::vector<Usage> newRangeUsages2;
  for (size_t i = position; i < range.usages.size(); i++) {
	newRangeUsages2.push_back(range.usages[i]);
  }
  Range newRange2{range.owner, newRangeUsages2.front().position, newRangeUsages2.back().position, newRangeUsages2};

  return std::make_pair(newRange1, newRange2);
}

void RegistersLinearScan::findProperSuccessors(uint64_t source, std::string variable,
											   std::set<uint64_t> &successors, std::set<uint64_t> &visited) {
  const auto sourceNode = flowGraph.getNode(source);
  bool found = false;
  for (const auto &var : sourceNode.getDefinedVariables()) {
	if (var == variable) {
	  found = true;
	  break;
	}
  }
  for (const auto &var : sourceNode.getUsedVariables()) {
	if (var == variable) {
	  found = true;
	  break;
	}
  }

  visited.insert(source);

  if (found) {
	successors.insert(source);
	return;
  }

  for (const auto &successor : sourceNode.getSuccessors()) {
	if (visited.find(successor) == visited.end()) {
	  findProperSuccessors(successor, variable, successors, visited);
	}
  }
}

void RegistersLinearScan::findProperPredecessors(uint64_t source,
												 std::string variable,
												 std::set<uint64_t> &predecessors,
												 std::set<uint64_t> &visited) {
  const auto sourceNode = flowGraph.getNode(source);
  bool found = false;
  for (const auto &var : sourceNode.getDefinedVariables()) {
	if (var == variable) {
	  found = true;
	  break;
	}
  }
  for (const auto &var : sourceNode.getUsedVariables()) {
	if (var == variable) {
	  found = true;
	  break;
	}
  }

  visited.insert(source);

  if (found) {
	predecessors.insert(source);
	return;
  }

  for (const auto &predecessor : sourceNode.getPredecessors()) {
	if (visited.find(predecessor) == visited.end()) {
	  findProperPredecessors(predecessor, variable, predecessors, visited);
	}
  }
}

bool RegistersLinearScan::rangesOverlap(const Range &range1, const Range &range2) {
  return range1.start <= range2.end && range1.end >= range2.start;
}

bool RegistersLinearScan::rangesInclude(const Range &outerRange, const Range &innerRange) {
  return outerRange.start <= innerRange.start && outerRange.end >= innerRange.end;
}

uint64_t RegistersLinearScan::rangeSize(const Range &range) {
  return range.end - range.start + 1;
}

std::vector<Usage> RegistersLinearScan::findRangeUsages(const std::string &variable, uint64_t position) {
  for (const auto &registerUsage : registersUsage) {
	for (const auto &range : registerUsage.second) {
	  if (range.owner == variable && range.start <= position && range.end >= position) {
		return range.usages;
	  }
	}
  }

  return {};
}

std::vector<Range> RegistersLinearScan::findRangesWithVariable(const std::string &variable) {
  std::vector<Range> result;
  for (const auto &registerUsage : registersUsage) {
	for (const auto &range : registerUsage.second) {
	  if (range.owner == variable/* && range.usages.front().type != UsageType::LVAL*/) {
		result.push_back(range);
	  }
	}
  }

  return result;
}

bool RegistersLinearScan::loadIsRequired(const Range &range) {
  if (range.usages.front().type != UsageType::LVAL) {
	return true;
  }

  return false;
}

bool RegistersLinearScan::storeIsRequired(const Range &range) {
  auto otherUsages = findRangesWithVariable(range.owner);
  for (const auto &otherUsage : otherUsages) {
	if (otherUsage.usages.front().type == UsageType::LVAL) {
	  continue;
	} else if (otherUsage.start != range.start || otherUsage.end != range.end) {
	  return true;
	} else if (otherUsage.start == range.start && otherUsage.end == range.end
		&& otherUsage.usages.front().type != UsageType::LVAL) {
	  return true;
	}
  }

  return false;
}

void RegistersLinearScan::printRanges() {
  for (auto &range : ranges) {
	uint64_t maxEnd = 128;
	std::cout << range.owner << "[" << range.start << "-" << range.end << "]: ";
	for (uint64_t i = 1; i < range.start; i++) {
	  std::cout << " ";
	}
	for (uint64_t i = range.start; i <= range.end; i++) {
	  std::cout << "#";
	}
	for (uint64_t i = range.end + 1; i < maxEnd; i++) {
	  std::cout << " ";
	}
	std::cout << std::endl;
  }
}

void RegistersLinearScan::printUsages() {
  for (auto &usage : variablesUsage) {
	std::cout << usage.first << ": ";
	for (auto &position : usage.second) {
	  std::cout << position.position;
	  if (position.type == UsageType::LVAL) {
		std::cout << "(L) ";
	  } else if (position.type == UsageType::RVAL) {
		std::cout << "(R) ";
	  } else {
		std::cout << "(B) ";
	  }
	}
	std::cout << std::endl;
  }
}

void RegistersLinearScan::printRegistersUsage() {
  for (auto &reg : registersUsage) {
	std::cout << Hardware::registerMap[reg.first].name << ": ";
	for (auto &range : reg.second) {
	  std::cout << range.owner << "(" << range.start << "-" << range.end << "): ";
	  for (auto &usage : range.usages) {
		std::cout << usage.position << " ";
	  }
	  std::cout << "| ";
	}
	std::cout << std::endl;
  }
}

void RegistersLinearScan::printVariableRegisters() {
  for (auto &variable : variableRegisters) {
	std::cout << variable.first << ": ";
	for (auto &position : variable.second) {
	  std::cout << position.first << "(" << position.second << ") ";
	}
	std::cout << std::endl;
  }
}
