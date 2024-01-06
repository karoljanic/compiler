#ifndef BASIC_INSTRUCTION_HPP
#define BASIC_INSTRUCTION_HPP

#include <iostream>
#include <memory>
#include <ostream>
#include <string>

#include "../core/Hardware.hpp"

class BasicInstruction {
 public:
  BasicInstruction();
  virtual ~BasicInstruction() = default;
  BasicInstruction(const BasicInstruction& from) = default;
  BasicInstruction& operator=(const BasicInstruction& from) = default;
  BasicInstruction(BasicInstruction&&) = default;
  BasicInstruction& operator=(BasicInstruction&&) = default;

  // virtual std::vector<HardwareInstruction> convertToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_HPP
