#ifndef CONTROL_FLOW_GRAPH_HPP
#define CONTROL_FLOW_GRAPH_HPP

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "../bblocks/BasicInstruction.hpp"

typedef std::vector<std::shared_ptr<BasicInstruction>> InstructionsBlock;

class ControlFlowGraphNode {
 private:
  static uint64_t idCounter;

  uint64_t id;
  std::vector<uint64_t> predecessors;
  std::vector<uint64_t> successors;
  std::set<std::string> usedVariables;
  std::set<std::string> definedVariables;
  std::set<std::string> liveInVariables;
  std::set<std::string> liveOutVariables;
  std::set<std::string> liveNowVariables;

  InstructionsBlock instructions;

 public:
  ControlFlowGraphNode();

  void addPredecessors(std::vector<uint64_t> predecessors);
  void addSuccessor(uint64_t successor);
  void setUsedVariables(const std::set<std::string>& variables);
  void setDefinedVariables(const std::set<std::string>& variable);
  void addInstruction(std::shared_ptr<BasicInstruction> instruction);
  void setLiveInVariables(const std::set<std::string>& liveInVariables);
  void setLiveOutVariables(const std::set<std::string>& liveOutVariables);
  void setLiveNowVariables(const std::set<std::string>& liveNowVariables);

  void expandBasicInstructions();

  uint64_t getId() const;
  const std::vector<uint64_t>& getPredecessors() const;
  const std::vector<uint64_t>& getSuccessors() const;
  const std::set<std::string>& getUsedVariables() const;
  const std::set<std::string>& getDefinedVariables() const;
  const std::set<std::string>& getLiveInVariables() const;
  const std::set<std::string>& getLiveOutVariables() const;
  const std::set<std::string>& getLiveNowVariables() const;
  const InstructionsBlock& getInstructions() const;
};

class ControlFlowGraph {
 public:
  ControlFlowGraph();

  void addSubgraph(const std::string& name, const std::vector<ControlFlowGraphNode>& subgraph);
  void initializeSuccessorsBasedOnPredecessors();
  void expandBasicInstructions();
  void calculateVariablesLiveRanges();
  void printGraph(std::ostream& stream) const;
  void printInstructions(std::ostream& stream) const;
  std::vector<std::pair<uint64_t, std::string>> getLiveInVariables() const;
  std::vector<std::pair<uint64_t, std::string>> getLiveOutVariables() const;
  std::vector<std::pair<uint64_t, std::string>> getLiveNowVariables() const;
  std::vector<std::pair<uint64_t, std::string>> getDefinedVariables() const;
  std::vector<std::pair<uint64_t, std::string>> getUsedVariables() const;
  const std::vector<ControlFlowGraphNode>& getInstructions(std::string subgraphName) const;

 private:
  std::map<std::string, std::vector<ControlFlowGraphNode>> graph;
};

#endif  // CONTROL_FLOW_GRAPH_HPP