#ifndef BASIC_INSTRUCTION_MATH_VV_HPP
#define BASIC_INSTRUCTION_MATH_VV_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMathVV : public BasicInstruction {
 private:
  BasicInstructionMathType operationType;
  std::string leftOperand;
  std::string rightOperand;
  std::string result;
  std::string temp1;
  std::string temp2;
  std::string temp3;

 public:
  BasicInstructionMathVV();
  BasicInstructionMathVV(BasicInstructionMathType operationType,
						 const std::string &leftOperand,
						 const std::string &rightOperand,
						 const std::string &result,
						 std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMathVV() = default;
  BasicInstructionMathVV(const BasicInstructionMathVV &from) = default;
  BasicInstructionMathVV &operator=(const BasicInstructionMathVV &from) = default;
  BasicInstructionMathVV(BasicInstructionMathVV &&) = default;
  BasicInstructionMathVV &operator=(BasicInstructionMathVV &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_MATH_VV_HPP