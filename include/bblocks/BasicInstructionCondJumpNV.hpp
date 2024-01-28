#ifndef BASIC_INSTRUCTION_COND_JUMP_NV_HPP
#define BASIC_INSTRUCTION_COND_JUMP_NV_HPP

#include "BasicInstruction.hpp"

class BasicInstructionCondJumpNV : public BasicInstruction {
 private:
  std::uint64_t leftNumber;
  std::string rightVariableName;
  BasicInstructionConditionType jumpCondition;
  std::string label;

 public:
  BasicInstructionCondJumpNV();
  BasicInstructionCondJumpNV(std::uint64_t leftNumber,
							 std::string rightVariableName,
							 BasicInstructionConditionType jumpCondition,
							 std::string label,
							 std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionCondJumpNV() = default;
  BasicInstructionCondJumpNV(const BasicInstructionCondJumpNV &from) = default;
  BasicInstructionCondJumpNV &operator=(const BasicInstructionCondJumpNV &from) = default;
  BasicInstructionCondJumpNV(BasicInstructionCondJumpNV &&) = default;
  BasicInstructionCondJumpNV &operator=(BasicInstructionCondJumpNV &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_COND_JUMP_NV_HPP