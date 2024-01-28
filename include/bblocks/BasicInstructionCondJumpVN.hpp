#ifndef BASIC_INSTRUCTION_COND_JUMP_VN_HPP
#define BASIC_INSTRUCTION_COND_JUMP_VN_HPP

#include "BasicInstruction.hpp"

class BasicInstructionCondJumpVN : public BasicInstruction {
 private:
  std::string leftVariableName;
  std::uint64_t rightNumber;
  BasicInstructionConditionType jumpCondition;
  std::string label;

 public:
  BasicInstructionCondJumpVN();
  BasicInstructionCondJumpVN(std::string leftVariableName,
							 std::uint64_t rightNumber,
							 BasicInstructionConditionType jumpCondition,
							 std::string label,
							 std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionCondJumpVN() = default;
  BasicInstructionCondJumpVN(const BasicInstructionCondJumpVN &from) = default;
  BasicInstructionCondJumpVN &operator=(const BasicInstructionCondJumpVN &from) = default;
  BasicInstructionCondJumpVN(BasicInstructionCondJumpVN &&) = default;
  BasicInstructionCondJumpVN &operator=(BasicInstructionCondJumpVN &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_COND_JUMP_VN_HPP