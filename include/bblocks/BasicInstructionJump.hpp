#ifndef BASIC_INSTRUCTION_JUMP_HPP
#define BASIC_INSTRUCTION_JUMP_HPP

#include "BasicInstruction.hpp"

class BasicInstructionJump : public BasicInstruction {
 private:
  std::string label;

 public:
  BasicInstructionJump();
  BasicInstructionJump(std::string label, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionJump() = default;
  BasicInstructionJump(const BasicInstructionJump& from) = default;
  BasicInstructionJump& operator=(const BasicInstructionJump& from) = default;
  BasicInstructionJump(BasicInstructionJump&&) = default;
  BasicInstructionJump& operator=(BasicInstructionJump&&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_JUMP_HPP
