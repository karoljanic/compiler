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
#include <vector>

#include "Hardware.hpp"

enum UsageType { RVAL, LVAL };

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
  std::map<std::string, std::vector<Usage>> variablesUsage;
  std::map<std::string, std::vector<uint64_t>> liveVariables;
  std::vector<Range> ranges;
  std::map<HardwareRegister, std::vector<Range>> registersUsage;
  std::map<std::string, std::map<uint64_t, std::string>> variableRegisters;
  std::map<uint64_t, std::vector<std::pair<HardwareInstruction, std::pair<std::string, uint64_t>>>> memoryInteractions;

 public:
  RegistersLinearScan();
  RegistersLinearScan(std::shared_ptr<Hardware> hardware);

  void addVariableLiveInfo(const std::string& variable, uint64_t position);
  void addVariableUsageInfo(const std::string& variable, uint64_t position, UsageType usageType);
  void createRanges();
  void allocateRegisters();
  std::string getVariableRegister(const std::string& variable, uint64_t position);
  std::vector<std::pair<HardwareInstruction, std::pair<std::string, uint64_t>>> getMemoryInteractions(uint64_t position);

  std::optional<HardwareRegister> getFreeRegister(const Range& newRange);
  HardwareRegister splitRanges(const Range& newRange);
  std::vector<uint64_t> getVariableLives(const std::string& variable, const Range& range);
  std::vector<Usage> getVariableUsage(const std::string& variable, UsageType usageType, const Range& range);
  std::vector<uint64_t> vectorsIntersection(const std::vector<Usage>& vector1, const std::vector<Usage>& vector2);
  bool rangesOverlap(const Range& range1, const Range& range2);
  bool rangesInclude(const Range& outerRange, const Range& innerRange);
  uint64_t rangeSize(const Range& range);
  void printRanges();
  void printLiveVariables();
  void printUsages();
};

#endif  // REGISTERS_LINEAR_SCAN_HPP