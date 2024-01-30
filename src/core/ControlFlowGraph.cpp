#include "../../include/core/ControlFlowGraph.hpp"

uint64_t ControlFlowGraphNode::idCounter = 1;

ControlFlowGraphNode::ControlFlowGraphNode() : id(idCounter) {
  idCounter++;
}

void ControlFlowGraphNode::addPredecessors(std::vector<uint64_t> preds) {
  for (const auto &pred : preds) {
	if (pred != 0) {
	  predecessors.push_back(pred);
	}
  }
}

void ControlFlowGraphNode::addSuccessor(uint64_t successor) {
  if (successor == 0) {
	return;
  }

  successors.push_back(successor);
}

void ControlFlowGraphNode::addLabel(const std::string &label) {
  labels.push_back(label);
}

void ControlFlowGraphNode::setUsedVariables(const std::set<std::string> &variables) {
  usedVariables.insert(variables.begin(), variables.end());
  //usedVariables = variables;
}

void ControlFlowGraphNode::setDefinedVariables(const std::set<std::string> &variables) {
  definedVariables.insert(variables.begin(), variables.end());
  //definedVariables = variables;
}

void ControlFlowGraphNode::setInstruction(std::shared_ptr<BasicInstruction> inst) {
  instruction = inst;
}

void ControlFlowGraphNode::setLiveInVariables(const std::set<std::string> &liveIn) {
  liveInVariables = liveIn;
}

void ControlFlowGraphNode::setLiveOutVariables(const std::set<std::string> &liveOut) {
  liveOutVariables = liveOut;
}

void ControlFlowGraphNode::setLiveNowVariables(const std::set<std::string> &liveNow) {
  liveNowVariables = liveNow;
}

void ControlFlowGraphNode::expand() {
  instruction->expandToHardwareInstructions();
}

void ControlFlowGraphNode::clear() {
  usedVariables.clear();
  definedVariables.clear();
  liveInVariables.clear();
  liveOutVariables.clear();
  liveNowVariables.clear();
}

uint64_t ControlFlowGraphNode::getId() const {
  return id;
}

const std::vector<uint64_t> &ControlFlowGraphNode::getPredecessors() const {
  return predecessors;
}

const std::vector<uint64_t> &ControlFlowGraphNode::getSuccessors() const {
  return successors;
}

const std::vector<std::string> &ControlFlowGraphNode::getLabels() const {
  return labels;
}

const std::set<std::string> &ControlFlowGraphNode::getUsedVariables() const {
  return usedVariables;
}

const std::set<std::string> &ControlFlowGraphNode::getDefinedVariables() const {
  return definedVariables;
}

const std::set<std::string> &ControlFlowGraphNode::getLiveInVariables() const {
  return liveInVariables;
}

const std::set<std::string> &ControlFlowGraphNode::getLiveOutVariables() const {
  return liveOutVariables;
}

const std::set<std::string> &ControlFlowGraphNode::getLiveNowVariables() const {
  return liveNowVariables;
}

const std::shared_ptr<BasicInstruction> &ControlFlowGraphNode::getInstruction() const {
  return instruction;
}

ControlFlowGraph::ControlFlowGraph() {}

void ControlFlowGraph::addSubgraph(const std::string &name, const std::vector<ControlFlowGraphNode> &subgraph) {
  graph[name] = subgraph;
}

void ControlFlowGraph::initializeSuccessorsBasedOnPredecessors() {
  for (auto &subgraph : graph) {
	for (auto &node : subgraph.second) {
	  for (auto &predecessor : node.getPredecessors()) {
		for (auto &node2 : subgraph.second) {
		  if (node2.getId() == predecessor) {
			node2.addSuccessor(node.getId());
		  }
		}
	  }
	}
  }
}

void ControlFlowGraph::optimize() {

}

void ControlFlowGraph::expandInstructions() {
  for (auto &subgraph : graph) {
	for (auto &node : subgraph.second) {
	  node.expand();
	}
  }
}

void ControlFlowGraph::clear() {
  for (auto &subgraph : graph) {
	for (auto &node : subgraph.second) {
	  node.clear();
	}
  }
}

void ControlFlowGraph::replace(const std::map<std::string, std::string>& uselessTemporariesMapping) {
  for (auto &subgraph : graph) {
	for (auto &node : subgraph.second) {
	  node.getInstruction()->replace(uselessTemporariesMapping);
	}
  }
}

std::map<std::string, std::vector<std::vector < uint64_t>>>
ControlFlowGraph::calculateVariablesLiveRanges() {
  // initialize used and defined variables
  for (auto &subgraph : graph) {
	for (auto &node : subgraph.second) {
	  std::set<std::string> used;
	  std::set<std::string> defined;
	  for (const auto &machineCode : node.getInstruction()->getMachineCode()) {
		const auto ins = machineCode.second.first;
		const auto arg = machineCode.second.second;
		if (Hardware::isRegName(arg)) {
		  continue;
		}
		switch (ins) {
		  case HardwareInstruction::READ: {
			break;
		  }
		  case HardwareInstruction::WRITE: {
			break;
		  }
		  case HardwareInstruction::LOAD: {
			if (defined.find(arg) == defined.end()) {
			  used.insert(arg);
			}
			break;
		  }
		  case HardwareInstruction::STORE: {
			if (defined.find(arg) == defined.end()) {
			  used.insert(arg);
			}
			break;
		  }
		  case HardwareInstruction::ADD: {
			if (defined.find(arg) == defined.end()) {
			  used.insert(arg);
			}
			break;
		  }
		  case HardwareInstruction::SUB: {
			if (defined.find(arg) == defined.end()) {
			  used.insert(arg);
			}
			break;
		  }
		  case HardwareInstruction::GET: {
			if (defined.find(arg) == defined.end()) {
			  used.insert(arg);
			}
			break;
		  }
		  case HardwareInstruction::PUT: {
			defined.insert(arg);
			break;
		  }
		  case HardwareInstruction::RST: {
			defined.insert(arg);
			break;
		  }
		  case HardwareInstruction::INC: {
			if (defined.find(arg) == defined.end()) {
			  used.insert(arg);
			}
			defined.insert(arg);
			break;
		  }
		  case HardwareInstruction::DEC: {
			if (defined.find(arg) == defined.end()) {
			  used.insert(arg);
			}
			defined.insert(arg);
			break;
		  }
		  case HardwareInstruction::SHL: {
			if (defined.find(arg) == defined.end()) {
			  used.insert(arg);
			}
			defined.insert(arg);
			break;
		  }
		  case HardwareInstruction::SHR: {
			if (defined.find(arg) == defined.end()) {
			  used.insert(arg);
			}
			defined.insert(arg);
			break;
		  }
		  case HardwareInstruction::JUMP: {
			break;
		  }
		  case HardwareInstruction::JPOS: {
			break;
		  }
		  case HardwareInstruction::JZERO: {
			break;
		  }
		  case HardwareInstruction::JUMPR: {
			if (defined.find(arg) == defined.end()) {
			  used.insert(arg);
			}
			break;
		  }
		  case HardwareInstruction::STRK: {
			defined.insert(arg);
			break;
		  }
		  case HardwareInstruction::HALT: {
			break;
		  }
		  case HardwareInstruction::LABEL: {
			break;
		  }
		  case HardwareInstruction::SAVE_STATE: {
			break;
		  }
		  case HardwareInstruction::RESTORE_STATE: {
			break;
		  }
		}
	  }
	  node.setDefinedVariables(defined);
	  node.setUsedVariables(used);
	}
  }

  // calculate live in and live out variables
  for (auto &subgraph : graph) {
	bool changed = true;
	while (changed) {
	  changed = false;
	  for (auto &node : subgraph.second) {
		std::set<std::string> newLiveInVariables;
		std::set<std::string> newLiveOutVariables;

		for (const auto &variable : node.getUsedVariables()) {
		  newLiveInVariables.insert(variable);
		}

		for (const auto &variable : node.getLiveOutVariables()) {
		  if (node.getDefinedVariables().find(variable) == node.getDefinedVariables().end()) {
			newLiveInVariables.insert(variable);
		  }
		}

		for (const auto &successor : node.getSuccessors()) {
		  for (auto &sNode : subgraph.second) {
			if (sNode.getId() == successor) {
			  for (const auto &variable : sNode.getLiveInVariables()) {
				newLiveOutVariables.insert(variable);
			  }
			}
		  }
		}

		if (newLiveInVariables != node.getLiveInVariables() || newLiveOutVariables != node.getLiveOutVariables()) {
		  changed = true;
		}

		node.setLiveInVariables(newLiveInVariables);
		node.setLiveOutVariables(newLiveOutVariables);
	  }
	}
  }

  // calculate live now variables
  for (auto &subgraph : graph) {
	for (auto &node : subgraph.second) {
	  std::set<std::string> newLiveNowVariables;
	  for (const auto &variable : node.getDefinedVariables()) {
		if (node.getLiveOutVariables().find(variable) == node.getLiveOutVariables().end()) {
		  newLiveNowVariables.insert(variable);
		}
	  }
	  node.setLiveNowVariables(newLiveNowVariables);
	}
  }

  std::map < std::string, std::vector < std::vector < uint64_t>>> result;
  for (const auto &subgraph : graph) {
	for (const auto &node : subgraph.second) {
	  const auto used = node.getUsedVariables();
	  for (const auto &var : node.getDefinedVariables()) {
		if (used.find(var) != used.end()) {
		  continue;
		}

		if (result.find(var) == result.end()) {
		  result[var] = std::vector < std::vector < uint64_t >> ();
		}
		result[var].push_back(std::vector < uint64_t > {});
	  }

	  auto successors = node.getSuccessors();
	  std::sort(successors.begin(), successors.end());
	  auto predecessors = node.getPredecessors();
	  std::sort(predecessors.begin(), predecessors.end());

	  std::set<std::string> vars{};
	  for (const auto &var : node.getLiveInVariables()) {
		vars.insert(var);
	  }
	  for (const auto &var : node.getLiveOutVariables()) {
		vars.insert(var);
	  }
	  for (const auto &var : node.getLiveNowVariables()) {
		vars.insert(var);
	  }

	  for (const auto &var : vars) {
		if (Hardware::isRegName(var)) {
		  continue;
		}

		if (result.find(var) == result.end()) {
		  result[var] = std::vector < std::vector < uint64_t >> ();
		  result[var].push_back(std::vector < uint64_t > {});
		}

		if (predecessors.size() > 1 || successors.size() > 1) {
		  result[var].push_back({node.getId()});
		}
		else if(predecessors.size() == 1 && (node.getId() - predecessors.front()) != 1) {
		  result[var].push_back({node.getId()});
		}
		else if(successors.size() == 1 && (successors.front() - node.getId()) != 1) {
		  result[var].push_back({node.getId()});
		}
		else {
		  result[var].back().push_back(node.getId());
		}
	  }
	}
  }

  return result;
}

void ControlFlowGraph::printGraph(std::ostream &stream) const {
  for (const auto &subgraph : graph) {
	stream << "Subgraph: " << subgraph.first << std::endl;
	for (const auto &node : subgraph.second) {
	  node.getInstruction()->print(stream);

	  stream << std::endl << "Id: " << node.getId();

	  const auto &labels = node.getLabels();
	  stream << std::endl << "Labels: ";
	  for (const auto &label : labels) {
		stream << label << " ";
	  }

	  stream << std::endl << "Predecessors: ";
	  for (const auto &predecessor : node.getPredecessors()) {
		stream << predecessor << " ";
	  }

	  stream << std::endl << "Successors: ";
	  for (const auto &successor : node.getSuccessors()) {
		stream << successor << " ";
	  }

	  stream << std::endl << "Used variables: ";
	  for (const auto &variable : node.getUsedVariables()) {
		stream << variable << " ";
	  }

	  stream << std::endl << "Defined variables: ";
	  for (const auto &variable : node.getDefinedVariables()) {
		stream << variable << " ";
	  }

	  stream << std::endl << "Live in variables: ";
	  for (const auto &variable : node.getLiveInVariables()) {
		stream << variable << " ";
	  }

	  stream << std::endl << "Live out variables: ";
	  for (const auto &variable : node.getLiveOutVariables()) {
		stream << variable << " ";
	  }

	  stream << std::endl << "Live now variables: ";
	  for (const auto &variable : node.getLiveNowVariables()) {
		stream << variable << " ";
	  }

	  stream << std::endl << std::endl;
	}
	stream << std::endl << std::endl << std::endl;
  }
}

void ControlFlowGraph::printInstructions(std::ostream &stream) const {
  for (const auto &subgraph : graph) {
	stream << "Subgraph: " << subgraph.first << std::endl;
	for (const auto &node : subgraph.second) {
	  const auto &labels = node.getLabels();
	  for (const auto &label : labels) {
		stream << label << " " << std::endl;
	  }
	  for (const auto &machineCode : node.getInstruction()->getMachineCode()) {
		stream << Hardware::instructionMap[machineCode.second.first].opcode << " " << machineCode.second.second
			   << std::endl;
	  }
	}
  }
}

std::vector<std::pair<uint64_t, std::string>> ControlFlowGraph::getLiveInVariables() const {
  std::vector<std::pair<uint64_t, std::string>> result;
  for (const auto &subgraph : graph) {
	for (const auto &node : subgraph.second) {
	  for (const auto &variable : node.getLiveInVariables()) {
		result.push_back({node.getId(), variable});
	  }
	}
  }

  return result;
}

std::vector<std::pair<uint64_t, std::string>> ControlFlowGraph::getLiveOutVariables() const {
  std::vector<std::pair<uint64_t, std::string>> result;
  for (const auto &subgraph : graph) {
	for (const auto &node : subgraph.second) {
	  for (const auto &variable : node.getLiveOutVariables()) {
		result.push_back({node.getId(), variable});
	  }
	}
  }

  return result;
}

std::vector<std::pair<uint64_t, std::string>> ControlFlowGraph::getLiveNowVariables() const {
  std::vector<std::pair<uint64_t, std::string>> result;
  for (const auto &subgraph : graph) {
	for (const auto &node : subgraph.second) {
	  for (const auto &variable : node.getLiveNowVariables()) {
		result.push_back({node.getId(), variable});
	  }
	}
  }

  return result;
}

std::vector<std::pair<uint64_t, std::string>> ControlFlowGraph::getDefinedVariables() const {
  std::vector<std::pair<uint64_t, std::string>> result;
  for (const auto &subgraph : graph) {
	for (const auto &node : subgraph.second) {
	  for (const auto &variable : node.getDefinedVariables()) {
		result.push_back({node.getId(), variable});
	  }
	}
  }

  return result;
}

std::vector<std::pair<uint64_t, std::string>> ControlFlowGraph::getUsedVariables() const {
  std::vector<std::pair<uint64_t, std::string>> result;
  for (const auto &subgraph : graph) {
	for (const auto &node : subgraph.second) {
	  for (const auto &variable : node.getUsedVariables()) {
		result.push_back({node.getId(), variable});
	  }
	}
  }

  return result;
}

ControlFlowGraphNode &ControlFlowGraph::getNode(uint64_t id) {
  for (auto &subgraph : graph) {
	for (auto &node : subgraph.second) {
	  if (node.getId() == id) {
		return node;
	  }
	}
  }

  return graph.at("main").at(0);
}

std::vector<std::string> ControlFlowGraph::getSubgraphNames() const {
  std::vector<std::string> result;
  for (const auto &subgraph : graph) {
	result.push_back(subgraph.first);
  }

  return result;
}

std::vector<ControlFlowGraphNode> &ControlFlowGraph::getNodes(std::string subgraphName) {
  return graph.at(subgraphName);
}
