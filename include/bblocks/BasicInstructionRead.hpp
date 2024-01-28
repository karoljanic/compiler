#ifndef BASIC_INSTRUCTION_READ_HPP
#define BASIC_INSTRUCTION_READ_HPP

#include "BasicInstruction.hpp"

class BasicInstructionRead : public BasicInstruction {
 private:
  std::string leftRegister;

 public:
  BasicInstructionRead();
  BasicInstructionRead(const std::string &variableName, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionRead() = default;
  BasicInstructionRead(const BasicInstructionRead &from) = default;
  BasicInstructionRead &operator=(const BasicInstructionRead &from) = default;
  BasicInstructionRead(BasicInstructionRead &&) = default;
  BasicInstructionRead &operator=(BasicInstructionRead &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_READ_HPP