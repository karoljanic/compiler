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

  const std::string &getLeftRegister() const { return leftRegister; }
  const std::string &getRightRegister() const { return rightRegister; }

  void setLeftRegister(const std::string &leftRegister) { this->leftRegister = leftRegister; }
  void setRightRegister(const std::string &rightRegister) { this->rightRegister = rightRegister; }

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_MOV_VV_HPP