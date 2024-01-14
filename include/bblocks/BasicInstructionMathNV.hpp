#ifndef BASIC_INSTRUCTION_MATH_NV_HPP
#define BASIC_INSTRUCTION_MATH_NV_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMathNV : public BasicInstruction {
 private:
  BasicInstructionMathType operationType;
  uint64_t leftOperand;
  std::string rightOperand;
  std::string result;

 public:
  BasicInstructionMathNV();
  BasicInstructionMathNV(BasicInstructionMathType operationType, uint64_t leftOperand, const std::string& rightOperand,
                         const std::string& result, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMathNV() = default;
  BasicInstructionMathNV(const BasicInstructionMathNV& from) = default;
  BasicInstructionMathNV& operator=(const BasicInstructionMathNV& from) = default;
  BasicInstructionMathNV(BasicInstructionMathNV&&) = default;
  BasicInstructionMathNV& operator=(BasicInstructionMathNV&&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_MATH_NV_HPP