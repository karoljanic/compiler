#ifndef BASIC_INSTRUCTION_JUMP_HPP
#define BASIC_INSTRUCTION_JUMP_HPP

#include "BasicInstruction.hpp"

enum class BasicInstructionConditionType { LESS, LESS_EQUAL, EQUAL, NOT_EQUAL };

class BasicInstructionLabel : public BasicInstruction {
 private:
  std::string label;

 public:
  BasicInstructionLabel();
  BasicInstructionLabel(std::string label);
  virtual ~BasicInstructionLabel() = default;
  BasicInstructionLabel(const BasicInstructionLabel& from) = default;
  BasicInstructionLabel& operator=(const BasicInstructionLabel& from) = default;
  BasicInstructionLabel(BasicInstructionLabel&&) = default;
  BasicInstructionLabel& operator=(BasicInstructionLabel&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionJump : public BasicInstruction {
 private:
  std::string label;

 public:
  BasicInstructionJump();
  BasicInstructionJump(std::string label);
  virtual ~BasicInstructionJump() = default;
  BasicInstructionJump(const BasicInstructionJump& from) = default;
  BasicInstructionJump& operator=(const BasicInstructionJump& from) = default;
  BasicInstructionJump(BasicInstructionJump&&) = default;
  BasicInstructionJump& operator=(BasicInstructionJump&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionConditionJumpVarVar : public BasicInstruction {
 private:
  std::string leftVariableName;
  std::string rightVariableName;
  BasicInstructionConditionType jumpCondition;
  std::string label;

 public:
  BasicInstructionConditionJumpVarVar();
  BasicInstructionConditionJumpVarVar(std::string leftVariableName, std::string rightVariableName,
                                      BasicInstructionConditionType jumpCondition, std::string label);
  virtual ~BasicInstructionConditionJumpVarVar() = default;
  BasicInstructionConditionJumpVarVar(const BasicInstructionConditionJumpVarVar& from) = default;
  BasicInstructionConditionJumpVarVar& operator=(const BasicInstructionConditionJumpVarVar& from) = default;
  BasicInstructionConditionJumpVarVar(BasicInstructionConditionJumpVarVar&&) = default;
  BasicInstructionConditionJumpVarVar& operator=(BasicInstructionConditionJumpVarVar&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionConditionJumpVarNum : public BasicInstruction {
 private:
  std::string leftVariableName;
  std::uint64_t rightNumber;
  BasicInstructionConditionType jumpCondition;
  std::string label;

 public:
  BasicInstructionConditionJumpVarNum();
  BasicInstructionConditionJumpVarNum(std::string leftVariableName, std::uint64_t rightNumber,
                                      BasicInstructionConditionType jumpCondition, std::string label);
  virtual ~BasicInstructionConditionJumpVarNum() = default;
  BasicInstructionConditionJumpVarNum(const BasicInstructionConditionJumpVarNum& from) = default;
  BasicInstructionConditionJumpVarNum& operator=(const BasicInstructionConditionJumpVarNum& from) = default;
  BasicInstructionConditionJumpVarNum(BasicInstructionConditionJumpVarNum&&) = default;
  BasicInstructionConditionJumpVarNum& operator=(BasicInstructionConditionJumpVarNum&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionConditionJumpNumVar : public BasicInstruction {
 private:
  std::uint64_t leftNumber;
  std::string rightVariableName;
  BasicInstructionConditionType jumpCondition;
  std::string label;

 public:
  BasicInstructionConditionJumpNumVar();
  BasicInstructionConditionJumpNumVar(std::uint64_t leftNumber, std::string rightVariableName,
                                      BasicInstructionConditionType jumpCondition, std::string label);
  virtual ~BasicInstructionConditionJumpNumVar() = default;
  BasicInstructionConditionJumpNumVar(const BasicInstructionConditionJumpNumVar& from) = default;
  BasicInstructionConditionJumpNumVar& operator=(const BasicInstructionConditionJumpNumVar& from) = default;
  BasicInstructionConditionJumpNumVar(BasicInstructionConditionJumpNumVar&&) = default;
  BasicInstructionConditionJumpNumVar& operator=(BasicInstructionConditionJumpNumVar&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionConditionJumpNumNum : public BasicInstruction {
 private:
  std::uint64_t leftNumber;
  std::uint64_t rightNumber;
  BasicInstructionConditionType jumpCondition;
  std::string label;

 public:
  BasicInstructionConditionJumpNumNum();
  BasicInstructionConditionJumpNumNum(std::uint64_t leftNumber, std::uint64_t rightNumber,
                                      BasicInstructionConditionType jumpCondition, std::string label);
  virtual ~BasicInstructionConditionJumpNumNum() = default;
  BasicInstructionConditionJumpNumNum(const BasicInstructionConditionJumpNumNum& from) = default;
  BasicInstructionConditionJumpNumNum& operator=(const BasicInstructionConditionJumpNumNum& from) = default;
  BasicInstructionConditionJumpNumNum(BasicInstructionConditionJumpNumNum&&) = default;
  BasicInstructionConditionJumpNumNum& operator=(BasicInstructionConditionJumpNumNum&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionJumpRelative : public BasicInstruction {
 private:
  std::string registerName;

 public:
  BasicInstructionJumpRelative();
  BasicInstructionJumpRelative(std::string variableName);
  virtual ~BasicInstructionJumpRelative() = default;
  BasicInstructionJumpRelative(const BasicInstructionJumpRelative& from) = default;
  BasicInstructionJumpRelative& operator=(const BasicInstructionJumpRelative& from) = default;
  BasicInstructionJumpRelative(BasicInstructionJumpRelative&&) = default;
  BasicInstructionJumpRelative& operator=(BasicInstructionJumpRelative&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_JUMP_HPP
