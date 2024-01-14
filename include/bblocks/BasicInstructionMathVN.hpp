#ifndef BASIC_INSTRUCTION_MATH_VN_HPP
#define BASIC_INSTRUCTION_MATH_VN_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMathVN : public BasicInstruction {
 private:
  BasicInstructionMathType operationType;
  std::string leftOperand;
  uint64_t rightOperand;
  std::string result;

 public:
  BasicInstructionMathVN();
  BasicInstructionMathVN(BasicInstructionMathType operationType, const std::string& leftOperand, uint64_t rightOperand,
                         const std::string& result, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMathVN() = default;
  BasicInstructionMathVN(const BasicInstructionMathVN& from) = default;
  BasicInstructionMathVN& operator=(const BasicInstructionMathVN& from) = default;
  BasicInstructionMathVN(BasicInstructionMathVN&&) = default;
  BasicInstructionMathVN& operator=(BasicInstructionMathVN&&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_MATH_VN_HPP