#ifndef BASIC_INSTRUCTION_IO_HPP
#define BASIC_INSTRUCTION_IO_HPP

#include <iostream>

#include "BasicInstruction.hpp"

class BasicInstructionRead : public BasicInstruction {
 private:
  std::string leftRegister;

 public:
  BasicInstructionRead();
  BasicInstructionRead(const std::string& variableName);
  virtual ~BasicInstructionRead() = default;
  BasicInstructionRead(const BasicInstructionRead& from) = default;
  BasicInstructionRead& operator=(const BasicInstructionRead& from) = default;
  BasicInstructionRead(BasicInstructionRead&&) = default;
  BasicInstructionRead& operator=(BasicInstructionRead&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionWrite : public BasicInstruction {
 private:
  std::string leftRegister;

 public:
  BasicInstructionWrite();
  BasicInstructionWrite(const std::string& variableName);
  virtual ~BasicInstructionWrite() = default;
  BasicInstructionWrite(const BasicInstructionWrite& from) = default;
  BasicInstructionWrite& operator=(const BasicInstructionWrite& from) = default;
  BasicInstructionWrite(BasicInstructionWrite&&) = default;
  BasicInstructionWrite& operator=(BasicInstructionWrite&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

class BasicInstructionHalt : public BasicInstruction {
 public:
  BasicInstructionHalt();
  virtual ~BasicInstructionHalt() = default;
  BasicInstructionHalt(const BasicInstructionHalt& from) = default;
  BasicInstructionHalt& operator=(const BasicInstructionHalt& from) = default;
  BasicInstructionHalt(BasicInstructionHalt&&) = default;
  BasicInstructionHalt& operator=(BasicInstructionHalt&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_IO_HPP