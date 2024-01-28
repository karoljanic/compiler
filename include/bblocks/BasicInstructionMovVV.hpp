#ifndef BASIC_INSTRUCTION_MOV_VV_HPP
#define BASIC_INSTRUCTION_MOV_VV_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMovVV : public BasicInstruction {
 private:
  std::string leftRegister;
  std::string rightRegister;

 public:
  BasicInstructionMovVV();
  BasicInstructionMovVV(const std::string &rightVariableName, const std::string &leftVariableName,
						std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMovVV() = default;
  BasicInstructionMovVV(const BasicInstructionMovVV &from) = default;
  BasicInstructionMovVV &operator=(const BasicInstructionMovVV &from) = default;
  BasicInstructionMovVV(BasicInstructionMovVV &&) = default;
  BasicInstructionMovVV &operator=(BasicInstructionMovVV &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_MOV_VV_HPP