#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <cstdint>
#include <map>
#include <string>
#include <utility>

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
  HALT
};

struct HardwareRegisterDescriptor {
  std::string name;
  uint64_t addressInMemory;
};

enum HardwareRegister { RA, RB, RC, RD, RE, RF, RG, RH };

/*
 * Memory description:
 * STACK for saving variables: 0 - STACK_SIZE (no-fixed size)
 * HEAP for dynamic memory allocation: STACK_SIZE+1 - 2^(62)-1
 */
class Hardware {
 private:
  uint64_t memoryOffset = 0;
  std::map<std::string, std::pair<uint64_t, uint64_t>> arrays;

 public:
  static std::map<HardwareInstruction, HardwareInstructionDescriptor> instructionMap;
  static std::map<HardwareRegister, HardwareRegisterDescriptor> registerMap;
  static uint64_t programCounterAddress;

  Hardware();

  void allocateArray(const std::string& arrayName, uint64_t size);
};

#endif  // HARDWARE_HPP
