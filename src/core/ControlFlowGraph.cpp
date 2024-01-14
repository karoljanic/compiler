#include "../../include/core/ControlFlowGraph.hpp"

uint64_t ControlFlowGraphNode::idCounter = 1;

ControlFlowGraphNode::ControlFlowGraphNode() : id(idCounter) {
  idCounter++;
}

void ControlFlowGraphNode::addPredecessors(std::vector<uint64_t> preds) {
  for (const auto& pred : preds) {
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

void ControlFlowGraphNode::setUsedVariables(const std::set<std::string>& variables) {
  usedVariables = variables;
}

void ControlFlowGraphNode::setDefinedVariables(const std::set<std::string>& variables) {
  definedVariables = variables;
}

void ControlFlowGraphNode::addInstruction(std::shared_ptr<BasicInstruction> instruction) {
  instructions.push_back(instruction);
}

void ControlFlowGraphNode::setLiveInVariables(const std::set<std::string>& liveIn) {
  liveInVariables = liveIn;
}

void ControlFlowGraphNode::setLiveOutVariables(const std::set<std::string>& liveOut) {
  liveOutVariables = liveOut;
}

void ControlFlowGraphNode::setLiveNowVariables(const std::set<std::string>& liveNow) {
  liveNowVariables = liveNow;
}

void ControlFlowGraphNode::expandBasicInstructions() {
  for (auto& instruction : instructions) {
    instruction->expandToHardwareInstructions();
  }
}

uint64_t ControlFlowGraphNode::getId() const {
  return id;
}

const std::vector<uint64_t>& ControlFlowGraphNode::getPredecessors() const {
  return predecessors;
}

const std::vector<uint64_t>& ControlFlowGraphNode::getSuccessors() const {
  return successors;
}

const std::set<std::string>& ControlFlowGraphNode::getUsedVariables() const {
  return usedVariables;
}

const std::set<std::string>& ControlFlowGraphNode::getDefinedVariables() const {
  return definedVariables;
}

const std::set<std::string>& ControlFlowGraphNode::getLiveInVariables() const {
  return liveInVariables;
}

const std::set<std::string>& ControlFlowGraphNode::getLiveOutVariables() const {
  return liveOutVariables;
}

const std::set<std::string>& ControlFlowGraphNode::getLiveNowVariables() const {
  return liveNowVariables;
}

const InstructionsBlock& ControlFlowGraphNode::getInstructions() const {
  return instructions;
}

ControlFlowGraph::ControlFlowGraph() {}

void ControlFlowGraph::addSubgraph(const std::string& name, const std::vector<ControlFlowGraphNode>& subgraph) {
  graph[name] = subgraph;
}

void ControlFlowGraph::initializeSuccessorsBasedOnPredecessors() {
  for (auto& subgraph : graph) {
    for (auto& node : subgraph.second) {
      for (auto& predecessor : node.getPredecessors()) {
        for (auto& node2 : subgraph.second) {
          if (node2.getId() == predecessor) {
            node2.addSuccessor(node.getId());
          }
        }
      }
    }
  }
}

void ControlFlowGraph::expandBasicInstructions() {
  for (auto& subgraph : graph) {
    for (auto& node : subgraph.second) {
      node.expandBasicInstructions();
    }
  }
}

void ControlFlowGraph::calculateVariablesLiveRanges() {
  // initialize used and defined variables
  for (auto& subgraph : graph) {
    for (auto& node : subgraph.second) {
      std::set<std::string> used;
      std::set<std::string> defined;
      for (auto& instruction : node.getInstructions()) {
        const auto& machineInstructions = instruction->getMachineCode();
        for (const auto& [ins, arg] : machineInstructions) {
          if (arg == Hardware::registerMap[Hardware::accumulator].name) {
            continue;
          }
          switch (ins) {
            case HardwareInstruction::READ: {
              //defined.insert(arg);
              break;
            }
            case HardwareInstruction::WRITE: {
              //if (defined.find(arg) == defined.end()) {
              //  used.insert(arg);
              //}
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
          }
        }
      }
      node.setDefinedVariables(defined);
      node.setUsedVariables(used);
    }
  }

  // calculate live in and live out variables
  for (auto& subgraph : graph) {
    bool changed = true;
    while (changed) {
      changed = false;
      for (auto& node : subgraph.second) {
        std::set<std::string> newLiveInVariables;
        std::set<std::string> newLiveOutVariables;

        for (const auto& variable : node.getUsedVariables()) {
          newLiveInVariables.insert(variable);
        }

        for (const auto& variable : node.getLiveOutVariables()) {
          if (node.getDefinedVariables().find(variable) == node.getDefinedVariables().end()) {
            newLiveInVariables.insert(variable);
          }
        }

        for (const auto& successor : node.getSuccessors()) {
          for (auto& sNode : subgraph.second) {
            if (sNode.getId() == successor) {
              for (const auto& variable : sNode.getLiveInVariables()) {
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
  for (auto& subgraph : graph) {
    for (auto& node : subgraph.second) {
      std::set<std::string> newLiveNowVariables;
      for (const auto& variable : node.getDefinedVariables()) {
        if (node.getLiveOutVariables().find(variable) == node.getLiveOutVariables().end()) {
          newLiveNowVariables.insert(variable);
        }
      }
      node.setLiveNowVariables(newLiveNowVariables);
    }
  }
}

void ControlFlowGraph::printGraph(std::ostream& stream) const {
  for (const auto& subgraph : graph) {
    stream << "Subgraph: " << subgraph.first << std::endl;
    for (const auto& node : subgraph.second) {
      stream << "Instructions: " << std::endl;
      for (size_t index = 0; index < node.getInstructions().size(); index++) {
        node.getInstructions()[index]->print(stream);
        if (index != node.getInstructions().size() - 1) {
          stream << std::endl;
        }
      }

      stream << std::endl << "Id: " << node.getId();

      stream << std::endl << "Predecessors: ";
      for (const auto& predecessor : node.getPredecessors()) {
        stream << predecessor << " ";
      }

      stream << std::endl << "Successors: ";
      for (const auto& successor : node.getSuccessors()) {
        stream << successor << " ";
      }

      stream << std::endl << "Used variables: ";
      for (const auto& variable : node.getUsedVariables()) {
        stream << variable << " ";
      }

      stream << std::endl << "Defined variables: ";
      for (const auto& variable : node.getDefinedVariables()) {
        stream << variable << " ";
      }

      stream << std::endl << "Live in variables: ";
      for (const auto& variable : node.getLiveInVariables()) {
        stream << variable << " ";
      }

      stream << std::endl << "Live out variables: ";
      for (const auto& variable : node.getLiveOutVariables()) {
        stream << variable << " ";
      }

      stream << std::endl << "Live now variables: ";
      for (const auto& variable : node.getLiveNowVariables()) {
        stream << variable << " ";
      }

      stream << std::endl << std::endl;
    }
    stream << std::endl << std::endl << std::endl;
  }
}

void ControlFlowGraph::printInstructions(std::ostream& stream) const {
  for (const auto& subgraph : graph) {
    stream << "Subgraph: " << subgraph.first << std::endl;
    for (const auto& node : subgraph.second) {
      for (const auto& instruction : node.getInstructions()) {
        const auto machineInstructions = instruction->getMachineCode();
        for (const auto& [instruction, argument] : machineInstructions) {
          stream << Hardware::instructionMap[instruction].opcode << " " << argument << std::endl;
        }
      }
    }
  }
}

std::vector<std::pair<uint64_t, std::string>> ControlFlowGraph::getLiveInVariables() const {
  std::vector<std::pair<uint64_t, std::string>> result;
  for (const auto& subgraph : graph) {
    for (const auto& node : subgraph.second) {
      for (const auto& variable : node.getLiveInVariables()) {
        result.push_back({node.getId(), variable});
      }
    }
  }

  return result;
}

std::vector<std::pair<uint64_t, std::string>> ControlFlowGraph::getLiveOutVariables() const {
  std::vector<std::pair<uint64_t, std::string>> result;
  for (const auto& subgraph : graph) {
    for (const auto& node : subgraph.second) {
      for (const auto& variable : node.getLiveOutVariables()) {
        result.push_back({node.getId(), variable});
      }
    }
  }

  return result;
}

std::vector<std::pair<uint64_t, std::string>> ControlFlowGraph::getLiveNowVariables() const {
  std::vector<std::pair<uint64_t, std::string>> result;
  for (const auto& subgraph : graph) {
    for (const auto& node : subgraph.second) {
      for (const auto& variable : node.getLiveNowVariables()) {
        result.push_back({node.getId(), variable});
      }
    }
  }

  return result;
}

std::vector<std::pair<uint64_t, std::string>> ControlFlowGraph::getDefinedVariables() const {
  std::vector<std::pair<uint64_t, std::string>> result;
  for (const auto& subgraph : graph) {
    for (const auto& node : subgraph.second) {
      for (const auto& variable : node.getDefinedVariables()) {
        result.push_back({node.getId(), variable});
      }
    }
  }

  return result;
}

std::vector<std::pair<uint64_t, std::string>> ControlFlowGraph::getUsedVariables() const {
  std::vector<std::pair<uint64_t, std::string>> result;
  for (const auto& subgraph : graph) {
    for (const auto& node : subgraph.second) {
      for (const auto& variable : node.getUsedVariables()) {
        result.push_back({node.getId(), variable});
      }
    }
  }

  return result;
}

const std::vector<ControlFlowGraphNode>& ControlFlowGraph::getInstructions(std::string subgraphName) const {
  return graph.at(subgraphName);
}