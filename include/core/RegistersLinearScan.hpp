#ifndef REGISTERS_LINEAR_SCAN_HPP
#define REGISTERS_LINEAR_SCAN_HPP

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <set>
#include <vector>

#include "Hardware.hpp"
#include "ControlFlowGraph.hpp"

enum UsageType { RVAL, LVAL, BOTH };

struct Usage {
  uint64_t position;
  UsageType type;
};

struct Range {
  std::string owner;
  uint64_t start;
  uint64_t end;
  std::vector<Usage> usages;
};

class RegistersLinearScan {
 private:
  std::shared_ptr<Hardware> hardware;
  ControlFlowGraph flowGraph;
  std::map<std::string, std::vector<std::vector<uint64_t>>> liveRanges;
  std::map<std::string, std::vector<Usage>> variablesUsage;
  std::vector<Range> ranges;
  std::map<HardwareRegister, std::vector<Range>> registersUsage;
  std::map<std::string, std::map<uint64_t, std::string>> variableRegisters;
  std::map<uint64_t, std::vector<std::pair<std::string, uint64_t>>> memoryStore;
  std::map<uint64_t, std::vector<std::pair<std::string, uint64_t>>> memoryLoad;
  std::map<std::string, uint64_t> variableMemoryPosition;

 public:
  RegistersLinearScan();
  RegistersLinearScan(std::shared_ptr<Hardware> hardware);

  void addVariableUsageInfo(const std::string& variable, uint64_t position, UsageType usageType);
  void createRanges(ControlFlowGraph& controlFlowGraph);
  void allocateRegisters(const std::vector<std::pair<std::string, uint64_t>>& requiredLoads,
						 const std::vector<std::pair<std::string, uint64_t>>& requiredStores);
  std::string getVariableRegister(const std::string& variable, uint64_t position);
  std::vector<std::pair<std::string, uint64_t>> getMemoryStore(uint64_t position);
  std::vector<std::pair<std::string, uint64_t>> getMemoryLoad(uint64_t position);

  private:
  std::optional<HardwareRegister> getFreeRegister(const Range& newRange);
  HardwareRegister splitRanges(const Range& newRange);
  std::vector<uint64_t> getVariableLives(const std::string& variable, const Range& range);
  std::vector<Usage> getVariableUsage(const std::string& variable, const Range& range);
  std::vector<uint64_t> vectorsIntersection(const std::vector<uint64_t>& vector1, const std::vector<uint64_t>& vector2);
  UsageType getVariableUsageType(const std::string& variable, uint64_t position);
  std::pair<Range, Range> divideRange1(const Range& range, uint64_t position);
  std::pair<Range, Range> divideRange2(const Range& range, uint64_t position);
  std::pair<Range, Range> divideRange3(const Range& range, uint64_t position);
  void findProperSuccessors(uint64_t source, std::string variable, std::set<uint64_t>& successors, std::set<uint64_t>& visited);
  void findProperPredecessors(uint64_t source, std::string variable, std::set<uint64_t>& predecessors, std::set<uint64_t>& visited);
  bool rangesOverlap(const Range& range1, const Range& range2);
  bool rangesInclude(const Range& outerRange, const Range& innerRange);
  uint64_t rangeSize(const Range& range);
  std::vector<Usage> findRangeUsages(const std::string& variable, uint64_t position);
  std::vector<Range> findRangesWithVariable(const std::string& variable);
  void printRanges();
  void printUsages();
  void printRegistersUsage();
  void printVariableRegisters();
};

#endif  // REGISTERS_LINEAR_SCAN_HPP