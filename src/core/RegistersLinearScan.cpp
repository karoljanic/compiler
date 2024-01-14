#include "../../include/core/RegistersLinearScan.hpp"

RegistersLinearScan::RegistersLinearScan() {}

RegistersLinearScan::RegistersLinearScan(std::shared_ptr<Hardware> hardware) : hardware(hardware) {}

void RegistersLinearScan::addVariableLiveInfo(const std::string& variable, uint64_t position) {
  if (liveVariables.find(variable) == liveVariables.end()) {
    liveVariables[variable] = std::vector<uint64_t>();
  }
  liveVariables[variable].push_back(position);
}

void RegistersLinearScan::addVariableUsageInfo(const std::string& variable, uint64_t position, UsageType usageType) {
  if (variablesUsage.find(variable) == variablesUsage.end()) {
    variablesUsage[variable] = std::vector<Usage>();
  }
  variablesUsage[variable].emplace_back(Usage{position, usageType});
}

void RegistersLinearScan::createRanges() {
  for (auto& liveVar : liveVariables) {
    std::sort(liveVar.second.begin(), liveVar.second.end());
    liveVar.second.erase(unique(liveVar.second.begin(), liveVar.second.end()), liveVar.second.end());
  }

  for (const auto& liveVar : liveVariables) {
    std::string variable = liveVar.first;
    std::vector<uint64_t> positions = liveVar.second;

    uint64_t start = positions[0];
    uint64_t end = positions[0];
    for (uint64_t i = 1; i < positions.size(); i++) {
      if (positions[i] == (end + 1)) {
        end = positions[i];
      }
      else {
        Range range{variable, start, end, {}};
        std::vector<Usage> lUsages = getVariableUsage(variable, UsageType::LVAL, range);
        std::vector<Usage> rUsages = getVariableUsage(variable, UsageType::RVAL, range);
        range.usages.insert(range.usages.end(), lUsages.begin(), lUsages.end());
        range.usages.insert(range.usages.end(), rUsages.begin(), rUsages.end());
        std::sort(range.usages.begin(), range.usages.end(),
                  [](const Usage& a, const Usage& b) { return a.position < b.position; });
        ranges.emplace_back(range);
        start = positions[i];
        end = positions[i];
      }
    }

    Range range{variable, start, end, {}};
    std::vector<Usage> lUsages = getVariableUsage(variable, UsageType::LVAL, range);
    std::vector<Usage> rUsages = getVariableUsage(variable, UsageType::RVAL, range);
    range.usages.insert(range.usages.end(), lUsages.begin(), lUsages.end());
    range.usages.insert(range.usages.end(), rUsages.begin(), rUsages.end());
    std::sort(range.usages.begin(), range.usages.end(), [](const Usage& a, const Usage& b) { return a.position < b.position; });
    ranges.emplace_back(range);
  }
}

void RegistersLinearScan::allocateRegisters() {
  registersUsage =
      std::map<HardwareRegister, std::vector<Range>>{{RB, {}}, {RC, {}}, {RD, {}}, {RE, {}}, {RF, {}}, {RG, {}}, {RH, {}}};

  std::sort(ranges.begin(), ranges.end(), [](const Range& a, const Range& b) { return a.start < b.start; });

  printRanges();
  printUsages();

  for (auto& range : ranges) {
    const auto freeReg = getFreeRegister(range);
    if (freeReg.has_value()) {
      const auto freeRegName = Hardware::registerMap[freeReg.value()].name;
      for (uint64_t i = range.start; i <= range.end; i++) {
        variableRegisters[range.owner][i] = freeRegName;
      }
      std::cout << "Found register for " << range.owner << ": " << freeRegName << std::endl;
    }
    else {
      const auto foundRegister = splitRanges(range);
      const auto freeRegName = Hardware::registerMap[foundRegister].name;
      for (uint64_t i = range.start; i <= range.end; i++) {
        variableRegisters[range.owner][i] = freeRegName;
      }
      std::cout << "Found* register for " << range.owner << ": " << freeRegName << std::endl;
    }
  }
}

std::string RegistersLinearScan::getVariableRegister(const std::string& variable, uint64_t position) {
  if (variableRegisters.find(variable) == variableRegisters.end()) {
    return variable;
  }

  if (variableRegisters[variable].find(position) == variableRegisters[variable].end()) {
    return variable;
  }

  return variableRegisters[variable][position];
}
std::vector<std::pair<HardwareInstruction, std::pair<std::string, uint64_t>>> RegistersLinearScan::getMemoryInteractions(
    uint64_t position) {
  if (memoryInteractions.find(position) == memoryInteractions.end()) {
    return {};
  }

  return memoryInteractions[position];
}

std::optional<HardwareRegister> RegistersLinearScan::getFreeRegister(const Range& newRange) {
  for (auto& reg : registersUsage) {
    const auto ranges = reg.second;

    bool anyOverlap = false;
    for (auto& range : ranges) {
      if (rangesOverlap(range, newRange)) {
        anyOverlap = true;
        break;
      }
    }

    if (!anyOverlap) {
      reg.second.push_back(newRange);
      std::sort(reg.second.begin(), reg.second.end(), [](const Range& a, const Range& b) { return a.start < b.start; });
      return reg.first;
    }
  }

  return std::nullopt;
}

HardwareRegister RegistersLinearScan::splitRanges(const Range& newRange) {
  const auto newRangeRUsages = getVariableUsage(newRange.owner, UsageType::RVAL, newRange);
  const auto newRangeLUsages = getVariableUsage(newRange.owner, UsageType::LVAL, newRange);
  auto newRangeUsages = std::vector<Usage>();
  newRangeUsages.insert(newRangeUsages.end(), newRangeRUsages.begin(), newRangeRUsages.end());
  newRangeUsages.insert(newRangeUsages.end(), newRangeLUsages.begin(), newRangeLUsages.end());

  uint64_t minConflictingUsagesNumber = std::numeric_limits<uint64_t>::max();
  HardwareRegister minConflictingRegister = RA;
  std::vector<Usage> minConflictingUsages;

  for (auto& reg : registersUsage) {
    std::vector<Usage> currentConflictingUsages;
    bool samePositions = false;
    for (auto& usageRange : reg.second) {
      const auto rUsages = getVariableUsage(usageRange.owner, UsageType::RVAL, newRange);
      const auto lUsages = getVariableUsage(usageRange.owner, UsageType::LVAL, newRange);
      auto conflictingUsages = std::vector<Usage>();
      conflictingUsages.insert(conflictingUsages.end(), rUsages.begin(), rUsages.end());
      conflictingUsages.insert(conflictingUsages.end(), lUsages.begin(), lUsages.end());

      if (!vectorsIntersection(conflictingUsages, newRangeUsages).empty()) {
        samePositions = true;
        break;
      }

      currentConflictingUsages.insert(currentConflictingUsages.end(), conflictingUsages.begin(), conflictingUsages.end());
    }

    if (!samePositions && currentConflictingUsages.size() < minConflictingUsagesNumber) {
      minConflictingUsagesNumber = currentConflictingUsages.size();
      minConflictingRegister = reg.first;
      minConflictingUsages = currentConflictingUsages;
    }
  }

  registersUsage[minConflictingRegister].push_back(newRange);

  std::cout << "Min conflicting variables: " << minConflictingUsagesNumber << ": ";
  for (auto& usage : minConflictingUsages) {
    std::cout << usage.position << " ";
  }
  std::cout << std::endl;
  std::cout << "Splitting " << Hardware::registerMap[minConflictingRegister].name << std::endl;

  const uint64_t memoryAddress1 = hardware->getFreeMemoryAddress();
  const uint64_t memoryAddress2 = hardware->getFreeMemoryAddress();

  for (const auto& range : registersUsage[minConflictingRegister]) {
    for (const auto& usage : range.usages) {
      if (usage.position >= newRange.start && usage.position <= newRange.end) {
        memoryInteractions[usage.position].push_back(std::make_pair(
            HardwareInstruction::STORE, std::make_pair(Hardware::registerMap[minConflictingRegister].name, memoryAddress2)));
        memoryInteractions[usage.position].push_back(std::make_pair(
            HardwareInstruction::LOAD, std::make_pair(Hardware::registerMap[minConflictingRegister].name, memoryAddress1)));

        memoryInteractions[usage.position + 1].push_back(std::make_pair(
            HardwareInstruction::STORE, std::make_pair(Hardware::registerMap[minConflictingRegister].name, memoryAddress1)));
        memoryInteractions[usage.position + 1].push_back(std::make_pair(
            HardwareInstruction::LOAD, std::make_pair(Hardware::registerMap[minConflictingRegister].name, memoryAddress2)));
      }
    }

    for (const auto& usage : newRange.usages) {
      if (usage.position >= range.start && usage.position <= range.end) {
        memoryInteractions[usage.position].push_back(std::make_pair(
            HardwareInstruction::STORE, std::make_pair(Hardware::registerMap[minConflictingRegister].name, memoryAddress1)));
        memoryInteractions[usage.position].push_back(std::make_pair(
            HardwareInstruction::LOAD, std::make_pair(Hardware::registerMap[minConflictingRegister].name, memoryAddress2)));

        memoryInteractions[usage.position + 1].push_back(std::make_pair(
            HardwareInstruction::STORE, std::make_pair(Hardware::registerMap[minConflictingRegister].name, memoryAddress2)));
        memoryInteractions[usage.position + 1].push_back(std::make_pair(
            HardwareInstruction::LOAD, std::make_pair(Hardware::registerMap[minConflictingRegister].name, memoryAddress1)));
      }
    }
  }

  return minConflictingRegister;
}

std::vector<uint64_t> RegistersLinearScan::getVariableLives(const std::string& variable, const Range& range) {
  std::vector<uint64_t> result;
  for (auto& liveVar : liveVariables[variable]) {
    if (liveVar >= range.start && liveVar <= range.end) {
      result.push_back(liveVar);
    }
  }
  std::sort(result.begin(), result.end());
  return result;
}

std::vector<Usage> RegistersLinearScan::getVariableUsage(const std::string& variable, UsageType usageType, const Range& range) {
  std::vector<Usage> result;
  for (auto& usage : variablesUsage[variable]) {
    if (usage.position >= range.start && usage.position <= range.end && usage.type == usageType) {
      result.push_back(usage);
    }
  }

  std::sort(result.begin(), result.end(), [](const Usage& a, const Usage& b) { return a.position < b.position; });
  return result;
}

std::vector<uint64_t> RegistersLinearScan::vectorsIntersection(const std::vector<Usage>& vector1,
                                                               const std::vector<Usage>& vector2) {
  std::vector<uint64_t> result;
  for (const Usage& usage1 : vector1) {
    for (const Usage& usage2 : vector2) {
      if (usage1.position == usage2.position) {
        result.push_back(usage1.position);
      }
    }
  }
  return result;
}

bool RegistersLinearScan::rangesOverlap(const Range& range1, const Range& range2) {
  return range1.start <= range2.end && range1.end >= range2.start;
}

bool RegistersLinearScan::rangesInclude(const Range& outerRange, const Range& innerRange) {
  return outerRange.start <= innerRange.start && outerRange.end >= innerRange.end;
}

uint64_t RegistersLinearScan::rangeSize(const Range& range) {
  return range.end - range.start + 1;
}

void RegistersLinearScan::printRanges() {
  for (auto& range : ranges) {
    uint64_t maxEnd = 128;
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

void RegistersLinearScan::printLiveVariables() {
  for (auto& liveVar : liveVariables) {
    std::cout << liveVar.first << ": ";
    for (auto& position : liveVar.second) {
      std::cout << position << " ";
    }
    std::cout << std::endl;
  }
}

void RegistersLinearScan::printUsages() {
  for (auto& usage : variablesUsage) {
    std::cout << usage.first << ": ";
    for (auto& position : usage.second) {
      std::cout << position.position;
      if (position.type == UsageType::LVAL) {
        std::cout << "(L) ";
      }
      else {
        std::cout << "(R) ";
      }
    }
    std::cout << std::endl;
  }
}
