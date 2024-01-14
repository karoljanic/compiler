#ifndef BASIC_INSTRUCTION_JUMP_RELATIVE_HPP
#define BASIC_INSTRUCTION_JUMP_RELATIVE_HPP

#include "BasicInstruction.hpp"

class BasicInstructionJumpRelative : public BasicInstruction {
 private:
  std::string rightVariableName;

 public:
  BasicInstructionJumpRelative();
  BasicInstructionJumpRelative(std::string variableName, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionJumpRelative() = default;
  BasicInstructionJumpRelative(const BasicInstructionJumpRelative& from) = default;
  BasicInstructionJumpRelative& operator=(const BasicInstructionJumpRelative& from) = default;
  BasicInstructionJumpRelative(BasicInstructionJumpRelative&&) = default;
  BasicInstructionJumpRelative& operator=(BasicInstructionJumpRelative&&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_JUMP_RELATIVE_HPP