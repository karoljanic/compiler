#ifndef BASIC_INSTRUCTION_MATH_NN_HPP
#define BASIC_INSTRUCTION_MATH_NN_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMathNN : public BasicInstruction {
 private:
  BasicInstructionMathType operationType;
  uint64_t leftOperand;
  uint64_t rightOperand;
  std::string result;

 public:
  BasicInstructionMathNN();
  BasicInstructionMathNN(BasicInstructionMathType operationType, uint64_t leftOperand, uint64_t rightOperand,
                         const std::string& result, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMathNN() = default;
  BasicInstructionMathNN(const BasicInstructionMathNN& from) = default;
  BasicInstructionMathNN& operator=(const BasicInstructionMathNN& from) = default;
  BasicInstructionMathNN(BasicInstructionMathNN&&) = default;
  BasicInstructionMathNN& operator=(BasicInstructionMathNN&&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_MATH_NN_HPP