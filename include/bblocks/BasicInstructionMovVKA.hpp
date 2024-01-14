#ifndef BASIC_INSTRUCTION_MOV_VKA_HPP
#define BASIC_INSTRUCTION_MOV_VKA_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMovVKA : public BasicInstruction {
 private:
  std::string rightRegister;
  std::string arrayName;
  uint64_t index;

 public:
  BasicInstructionMovVKA();
  BasicInstructionMovVKA(const std::string& variableName, const std::string& arrayName, uint64_t indexValue,
                         std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMovVKA() = default;
  BasicInstructionMovVKA(const BasicInstructionMovVKA& from) = default;
  BasicInstructionMovVKA& operator=(const BasicInstructionMovVKA& from) = default;
  BasicInstructionMovVKA(BasicInstructionMovVKA&&) = default;
  BasicInstructionMovVKA& operator=(BasicInstructionMovVKA&&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_MOV_VKA_HPP