#ifndef BASIC_INSTRUCTION_MOV_VA_HPP
#define BASIC_INSTRUCTION_MOV_VA_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMovVA : public BasicInstruction {
 private:
  std::string rightRegister;
  std::string arrayName;
  std::string indexRegister;
  bool indirectArrayBeginAddress;

 public:
  BasicInstructionMovVA();
  BasicInstructionMovVA(const std::string &variableName, const std::string &arrayName,
						const std::string &indexVariableName, bool indirectArray,
						std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMovVA() = default;
  BasicInstructionMovVA(const BasicInstructionMovVA &from) = default;
  BasicInstructionMovVA &operator=(const BasicInstructionMovVA &from) = default;
  BasicInstructionMovVA(BasicInstructionMovVA &&) = default;
  BasicInstructionMovVA &operator=(BasicInstructionMovVA &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_MOV_VA_HPP