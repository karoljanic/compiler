#ifndef BASIC_INSTRUCTION_STACK_HPP
#define BASIC_INSTRUCTION_STACK_HPP

#include <cstdint>
#include <vector>

#include "../../include/bblocks/BasicInstruction.hpp"

class BasicInstructionPush : public BasicInstruction {
 private:
  uint64_t begin;
  std::vector<std::string> source;

 public:
  BasicInstructionPush();
  BasicInstructionPush(uint64_t beginAddress, const std::vector<std::string>& source);
  virtual ~BasicInstructionPush() = default;
  BasicInstructionPush(const BasicInstructionPush& from) = default;
  BasicInstructionPush& operator=(const BasicInstructionPush& from) = default;
  BasicInstructionPush(BasicInstructionPush&&) = default;
  BasicInstructionPush& operator=(BasicInstructionPush&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionPop : public BasicInstruction {
 private:
  uint64_t begin;
  std::vector<std::string> destination;

 public:
  BasicInstructionPop();
  BasicInstructionPop(uint64_t beginAddress, const std::vector<std::string>& destination);
  virtual ~BasicInstructionPop() = default;
  BasicInstructionPop(const BasicInstructionPop& from) = default;
  BasicInstructionPop& operator=(const BasicInstructionPop& from) = default;
  BasicInstructionPop(BasicInstructionPop&&) = default;
  BasicInstructionPop& operator=(BasicInstructionPop&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_STACK_HPP