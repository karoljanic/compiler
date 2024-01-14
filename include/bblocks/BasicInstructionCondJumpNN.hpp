#ifndef BASIC_INSTRUCTION_COND_JUMP_NN_HPP
#define BASIC_INSTRUCTION_COND_JUMP_NN_HPP

#include "BasicInstruction.hpp"

class BasicInstructionCondJumpNN : public BasicInstruction {
 private:
  std::uint64_t leftNumber;
  std::uint64_t rightNumber;
  BasicInstructionConditionType jumpCondition;
  std::string label;

 public:
  BasicInstructionCondJumpNN();
  BasicInstructionCondJumpNN(std::uint64_t leftNumber, std::uint64_t rightNumber, BasicInstructionConditionType jumpCondition,
                             std::string label, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionCondJumpNN() = default;
  BasicInstructionCondJumpNN(const BasicInstructionCondJumpNN& from) = default;
  BasicInstructionCondJumpNN& operator=(const BasicInstructionCondJumpNN& from) = default;
  BasicInstructionCondJumpNN(BasicInstructionCondJumpNN&&) = default;
  BasicInstructionCondJumpNN& operator=(BasicInstructionCondJumpNN&&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_COND_JUMP_NN_HPP