#ifndef BASIC_INSTRUCTION_COND_JUMP_VV_HPP
#define BASIC_INSTRUCTION_COND_JUMP_VV_HPP

#include "BasicInstruction.hpp"

class BasicInstructionCondJumpVV : public BasicInstruction {
 private:
  std::string leftVariableName;
  std::string rightVariableName;
  BasicInstructionConditionType jumpCondition;
  std::string label;

 public:
  BasicInstructionCondJumpVV();
  BasicInstructionCondJumpVV(std::string leftVariableName,
							 std::string rightVariableName,
							 BasicInstructionConditionType jumpCondition,
							 std::string label,
							 std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionCondJumpVV() = default;
  BasicInstructionCondJumpVV(const BasicInstructionCondJumpVV &from) = default;
  BasicInstructionCondJumpVV &operator=(const BasicInstructionCondJumpVV &from) = default;
  BasicInstructionCondJumpVV(BasicInstructionCondJumpVV &&) = default;
  BasicInstructionCondJumpVV &operator=(BasicInstructionCondJumpVV &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  //BASIC_INSTRUCTION_COND_JUMP_VV_HPP