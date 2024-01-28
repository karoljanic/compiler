#ifndef BASIC_INSTRUCTION_MOV_NV_HPP
#define BASIC_INSTRUCTION_MOV_NV_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMovNV : public BasicInstruction {
 private:
  uint64_t numValue;
  std::string leftRegister;

 public:
  BasicInstructionMovNV();
  BasicInstructionMovNV(uint64_t numValue, const std::string &variableName, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMovNV() = default;
  BasicInstructionMovNV(const BasicInstructionMovNV &from) = default;
  BasicInstructionMovNV &operator=(const BasicInstructionMovNV &from) = default;
  BasicInstructionMovNV(BasicInstructionMovNV &&) = default;
  BasicInstructionMovNV &operator=(BasicInstructionMovNV &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_MOV_NV_HPP