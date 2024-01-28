#ifndef BASIC_INSTRUCTION_HPP
#define BASIC_INSTRUCTION_HPP

#include <iostream>
#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <map>

#include "../core/Hardware.hpp"
#include "../core/Utils.hpp"

enum class BasicInstructionMathType { UNDEFINED, ADD, SUB, MUL, DIV, MOD };

enum class BasicInstructionConditionType { LESS, LESS_EQUAL, EQUAL, NOT_EQUAL };

class BasicInstruction {
 protected:
  std::vector<MachineCodeType> machineCode;
  std::shared_ptr<Hardware> hardware;

 public:
  BasicInstruction();
  BasicInstruction(std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstruction() = default;
  BasicInstruction(const BasicInstruction &from) = default;
  BasicInstruction &operator=(const BasicInstruction &from) = default;
  BasicInstruction(BasicInstruction &&) = default;
  BasicInstruction &operator=(BasicInstruction &&) = default;

  const std::vector<MachineCodeType> &getMachineCode() const;

  size_t estimateMachineCodeSize();
  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_HPP
