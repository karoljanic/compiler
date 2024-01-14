#ifndef BASIC_INSTRUCTION_WRITE_HPP
#define BASIC_INSTRUCTION_WRITE_HPP

#include "BasicInstruction.hpp"

class BasicInstructionWrite : public BasicInstruction {
 private:
  std::string rightRegister;

 public:
  BasicInstructionWrite();
  BasicInstructionWrite(const std::string& variableName, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionWrite() = default;
  BasicInstructionWrite(const BasicInstructionWrite& from) = default;
  BasicInstructionWrite& operator=(const BasicInstructionWrite& from) = default;
  BasicInstructionWrite(BasicInstructionWrite&&) = default;
  BasicInstructionWrite& operator=(BasicInstructionWrite&&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_WRITE_HPP