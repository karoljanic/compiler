#ifndef BASIC_INSTRUCTION_HALT_HPP
#define BASIC_INSTRUCTION_HALT_HPP

#include "BasicInstruction.hpp"

class BasicInstructionHalt : public BasicInstruction {
 public:
  BasicInstructionHalt();
  BasicInstructionHalt(std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionHalt() = default;
  BasicInstructionHalt(const BasicInstructionHalt &from) = default;
  BasicInstructionHalt &operator=(const BasicInstructionHalt &from) = default;
  BasicInstructionHalt(BasicInstructionHalt &&) = default;
  BasicInstructionHalt &operator=(BasicInstructionHalt &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_HALT_HPP