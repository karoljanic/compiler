#ifndef BASIC_INSTRUCTION_LABEL_HPP
#define BASIC_INSTRUCTION_LABEL_HPP

#include "BasicInstruction.hpp"

class BasicInstructionLabel : public BasicInstruction {
 private:
  std::string label;

 public:
  BasicInstructionLabel();
  BasicInstructionLabel(std::string label, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionLabel() = default;
  BasicInstructionLabel(const BasicInstructionLabel &from) = default;
  BasicInstructionLabel &operator=(const BasicInstructionLabel &from) = default;
  BasicInstructionLabel(BasicInstructionLabel &&) = default;
  BasicInstructionLabel &operator=(BasicInstructionLabel &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_LABEL_HPP