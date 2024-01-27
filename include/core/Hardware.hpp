#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#undef BEGIN_ON_POWER_OF_TWO_BOUNDARY

struct HardwareInstructionDescriptor {
  std::string opcode;
  uint8_t cost;
};

enum HardwareInstruction {
  READ,
  WRITE,
  LOAD,
  STORE,
  ADD,
  SUB,
  GET,
  PUT,
  RST,
  INC,
  DEC,
  SHL,
  SHR,
  JUMP,
  JPOS,
  JZERO,
  STRK,
  JUMPR,
  HALT,
  LABEL,
  SAVE_STATE,
  RESTORE_STATE,
};

struct HardwareRegisterDescriptor {
  std::string name;
  uint64_t addressInMemory;
};

enum HardwareRegister { RA, RB, RC, RD, RE, RF, RG, RH };

/*
 * Memory description:
 * HEAP for saving arrays: 0 - SUM(arrays.size)
 * STACK for saving variables and passing parameters: SUM(arrays.size)+1 - 2^(62)-1
 */
class Hardware {
 private:
  std::uint64_t tempsCounter = 0;
  std::uint64_t labelsCounter = 0;
  uint64_t memoryOffset = 0;
  std::map<std::string, std::pair<uint64_t, uint64_t>> arrays;
  std::map<std::string, std::pair<uint64_t, uint64_t>> functions;

 public:
  static std::map<HardwareInstruction, HardwareInstructionDescriptor> instructionMap;
  static size_t registersNumber;
  static std::map<HardwareRegister, HardwareRegisterDescriptor> registerMap;
  static HardwareRegister accumulator;
  static bool isRegName(const std::string &name);

  Hardware();

  std::string getTempRegister();
  std::string getLabel(const std::string &scopeName, const std::string &labelName, bool incrementCounter);
  std::string getGlobalLabel(const std::string &scopeName, const std::string &labelName);
  void allocateArray(const std::string &arrayName, uint64_t size);
  void allocateFunctionRecord(const std::string &functionName, uint64_t size);
  uint64_t allocateStateRecord(uint64_t recordSize);
  uint64_t getArrayAddress(const std::string &arrayName);
  uint64_t getFunctionRecordAddress(const std::string &functionName);
  uint64_t getStackOffset();
  uint64_t getFreeMemoryAddress();

 private:
  uint64_t nearestPowerOfTwo(uint64_t n);
};

#endif  // HARDWARE_HPP
