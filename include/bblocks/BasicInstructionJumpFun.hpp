#ifndef BASIC_INSTRUCTION_JUMP_FUN_HPP
#define BASIC_INSTRUCTION_JUMP_FUN_HPP

#include "BasicInstruction.hpp"

class BasicInstructionJumpFun : public BasicInstruction {
 private:
  std::string name;
  uint64_t returnAddress;

 public:
  BasicInstructionJumpFun();
  BasicInstructionJumpFun(std::string functionName, uint64_t retAddress, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionJumpFun() = default;
  BasicInstructionJumpFun(const BasicInstructionJumpFun &from) = default;
  BasicInstructionJumpFun &operator=(const BasicInstructionJumpFun &from) = default;
  BasicInstructionJumpFun(BasicInstructionJumpFun &&) = default;
  BasicInstructionJumpFun &operator=(BasicInstructionJumpFun &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_JUMP_FUN_HPP
