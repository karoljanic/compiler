#ifndef BASIC_INSTRUCTION_MATH_HPP
#define BASIC_INSTRUCTION_MATH_HPP

#include "BasicInstruction.hpp"

enum class BasicInstructionMathOperationType { UNDEFINED, ADD, SUB, MUL, DIV, MOD };

class BasicInstructionMathOperationVarVar : public BasicInstruction {
 private:
  BasicInstructionMathOperationType operationType;
  std::string leftOperand;
  std::string rightOperand;
  std::string result;

 public:
  BasicInstructionMathOperationVarVar();
  BasicInstructionMathOperationVarVar(BasicInstructionMathOperationType operationType, const std::string& leftOperand,
                                      const std::string& rightOperand, const std::string& result,
                                      std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMathOperationVarVar() = default;
  BasicInstructionMathOperationVarVar(const BasicInstructionMathOperationVarVar& from) = default;
  BasicInstructionMathOperationVarVar& operator=(const BasicInstructionMathOperationVarVar& from) = default;
  BasicInstructionMathOperationVarVar(BasicInstructionMathOperationVarVar&&) = default;
  BasicInstructionMathOperationVarVar& operator=(BasicInstructionMathOperationVarVar&&) = default;

  virtual std::vector<std::pair<HardwareInstruction, std::string>> expandToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionMathOperationVarNum : public BasicInstruction {
 private:
  BasicInstructionMathOperationType operationType;
  std::string leftOperand;
  uint64_t rightOperand;
  std::string result;

 public:
  BasicInstructionMathOperationVarNum();
  BasicInstructionMathOperationVarNum(BasicInstructionMathOperationType operationType, const std::string& leftOperand,
                                      uint64_t rightOperand, const std::string& result, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMathOperationVarNum() = default;
  BasicInstructionMathOperationVarNum(const BasicInstructionMathOperationVarNum& from) = default;
  BasicInstructionMathOperationVarNum& operator=(const BasicInstructionMathOperationVarNum& from) = default;
  BasicInstructionMathOperationVarNum(BasicInstructionMathOperationVarNum&&) = default;
  BasicInstructionMathOperationVarNum& operator=(BasicInstructionMathOperationVarNum&&) = default;

  virtual std::vector<std::pair<HardwareInstruction, std::string>> expandToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionMathOperationNumVar : public BasicInstruction {
 private:
  BasicInstructionMathOperationType operationType;
  uint64_t leftOperand;
  std::string rightOperand;
  std::string result;

 public:
  BasicInstructionMathOperationNumVar();
  BasicInstructionMathOperationNumVar(BasicInstructionMathOperationType operationType, uint64_t leftOperand,
                                      const std::string& rightOperand, const std::string& result,
                                      std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMathOperationNumVar() = default;
  BasicInstructionMathOperationNumVar(const BasicInstructionMathOperationNumVar& from) = default;
  BasicInstructionMathOperationNumVar& operator=(const BasicInstructionMathOperationNumVar& from) = default;
  BasicInstructionMathOperationNumVar(BasicInstructionMathOperationNumVar&&) = default;
  BasicInstructionMathOperationNumVar& operator=(BasicInstructionMathOperationNumVar&&) = default;

  virtual std::vector<std::pair<HardwareInstruction, std::string>> expandToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionMathOperationNumNum : public BasicInstruction {
 private:
  BasicInstructionMathOperationType operationType;
  uint64_t leftOperand;
  uint64_t rightOperand;
  std::string result;

 public:
  BasicInstructionMathOperationNumNum();
  BasicInstructionMathOperationNumNum(BasicInstructionMathOperationType operationType, uint64_t leftOperand,
                                      uint64_t rightOperand, const std::string& result, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMathOperationNumNum() = default;
  BasicInstructionMathOperationNumNum(const BasicInstructionMathOperationNumNum& from) = default;
  BasicInstructionMathOperationNumNum& operator=(const BasicInstructionMathOperationNumNum& from) = default;
  BasicInstructionMathOperationNumNum(BasicInstructionMathOperationNumNum&&) = default;
  BasicInstructionMathOperationNumNum& operator=(BasicInstructionMathOperationNumNum&&) = default;

  virtual std::vector<std::pair<HardwareInstruction, std::string>> expandToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_MATH_HPP
