#ifndef BASIC_INSTRUCTION_MOV_HPP
#define BASIC_INSTRUCTION_MOV_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMovVarToVar : public BasicInstruction {
 private:
  std::string leftRegister;
  std::string rightRegister;

 public:
  BasicInstructionMovVarToVar();
  BasicInstructionMovVarToVar(const std::string& variable1Name, const std::string& variable2Name);
  virtual ~BasicInstructionMovVarToVar() = default;
  BasicInstructionMovVarToVar(const BasicInstructionMovVarToVar& from) = default;
  BasicInstructionMovVarToVar& operator=(const BasicInstructionMovVarToVar& from) = default;
  BasicInstructionMovVarToVar(BasicInstructionMovVarToVar&&) = default;
  BasicInstructionMovVarToVar& operator=(BasicInstructionMovVarToVar&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionMovVarToArr : public BasicInstruction {
 private:
  std::string leftRegister;
  std::string arrayName;
  std::string indexRegister;

 public:
  BasicInstructionMovVarToArr();
  BasicInstructionMovVarToArr(const std::string& variableName, const std::string& arrayName,
                              const std::string& indexVariableName);
  virtual ~BasicInstructionMovVarToArr() = default;
  BasicInstructionMovVarToArr(const BasicInstructionMovVarToArr& from) = default;
  BasicInstructionMovVarToArr& operator=(const BasicInstructionMovVarToArr& from) = default;
  BasicInstructionMovVarToArr(BasicInstructionMovVarToArr&&) = default;
  BasicInstructionMovVarToArr& operator=(BasicInstructionMovVarToArr&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionMovVarToKnownArr : public BasicInstruction {
 private:
  std::string leftRegister;
  std::string arrayName;
  uint64_t index;

 public:
  BasicInstructionMovVarToKnownArr();
  BasicInstructionMovVarToKnownArr(const std::string& variableName, const std::string& arrayName, uint64_t indexValue);
  virtual ~BasicInstructionMovVarToKnownArr() = default;
  BasicInstructionMovVarToKnownArr(const BasicInstructionMovVarToKnownArr& from) = default;
  BasicInstructionMovVarToKnownArr& operator=(const BasicInstructionMovVarToKnownArr& from) = default;
  BasicInstructionMovVarToKnownArr(BasicInstructionMovVarToKnownArr&&) = default;
  BasicInstructionMovVarToKnownArr& operator=(BasicInstructionMovVarToKnownArr&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionMovArrToVar : public BasicInstruction {
 private:
  std::string arrayName;
  std::string indexRegister;
  std::string rightRegister;

 public:
  BasicInstructionMovArrToVar();
  BasicInstructionMovArrToVar(const std::string& arrayName, const std::string& indexVariableName,
                              const std::string& variableName);
  virtual ~BasicInstructionMovArrToVar() = default;
  BasicInstructionMovArrToVar(const BasicInstructionMovArrToVar& from) = default;
  BasicInstructionMovArrToVar& operator=(const BasicInstructionMovArrToVar& from) = default;
  BasicInstructionMovArrToVar(BasicInstructionMovArrToVar&&) = default;
  BasicInstructionMovArrToVar& operator=(BasicInstructionMovArrToVar&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionMovKnownArrToVar : public BasicInstruction {
 private:
  std::string arrayName;
  uint64_t index;
  std::string rightRegister;

 public:
  BasicInstructionMovKnownArrToVar();
  BasicInstructionMovKnownArrToVar(const std::string& arrayName, uint64_t indexValue, const std::string& variableName);
  virtual ~BasicInstructionMovKnownArrToVar() = default;
  BasicInstructionMovKnownArrToVar(const BasicInstructionMovKnownArrToVar& from) = default;
  BasicInstructionMovKnownArrToVar& operator=(const BasicInstructionMovKnownArrToVar& from) = default;
  BasicInstructionMovKnownArrToVar(BasicInstructionMovKnownArrToVar&&) = default;
  BasicInstructionMovKnownArrToVar& operator=(BasicInstructionMovKnownArrToVar&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionMovVarToMem : public BasicInstruction {
 private:
  std::string leftRegister;
  std::string memoryAddress;

 public:
  BasicInstructionMovVarToMem();
  BasicInstructionMovVarToMem(const std::string& variableName, const std::string& memoryAddress);
  virtual ~BasicInstructionMovVarToMem() = default;
  BasicInstructionMovVarToMem(const BasicInstructionMovVarToMem& from) = default;
  BasicInstructionMovVarToMem& operator=(const BasicInstructionMovVarToMem& from) = default;
  BasicInstructionMovVarToMem(BasicInstructionMovVarToMem&&) = default;
  BasicInstructionMovVarToMem& operator=(BasicInstructionMovVarToMem&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionMovMemToVar : public BasicInstruction {
 private:
  std::string memoryAddress;
  std::string rightRegister;

 public:
  BasicInstructionMovMemToVar();
  BasicInstructionMovMemToVar(const std::string& memoryAddress, const std::string& variableName);
  virtual ~BasicInstructionMovMemToVar() = default;
  BasicInstructionMovMemToVar(const BasicInstructionMovMemToVar& from) = default;
  BasicInstructionMovMemToVar& operator=(const BasicInstructionMovMemToVar& from) = default;
  BasicInstructionMovMemToVar(BasicInstructionMovMemToVar&&) = default;
  BasicInstructionMovMemToVar& operator=(BasicInstructionMovMemToVar&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionMovNumToVar : public BasicInstruction {
 private:
  uint64_t numValue;
  std::string leftRegister;

 public:
  BasicInstructionMovNumToVar();
  BasicInstructionMovNumToVar(uint64_t numValue, const std::string& variableName);
  virtual ~BasicInstructionMovNumToVar() = default;
  BasicInstructionMovNumToVar(const BasicInstructionMovNumToVar& from) = default;
  BasicInstructionMovNumToVar& operator=(const BasicInstructionMovNumToVar& from) = default;
  BasicInstructionMovNumToVar(BasicInstructionMovNumToVar&&) = default;
  BasicInstructionMovNumToVar& operator=(BasicInstructionMovNumToVar&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_MOV_HPP
