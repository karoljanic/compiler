#ifndef BASIC_INSTRUCTION_SAVE_STATE_HPP
#define BASIC_INSTRUCTION_SAVE_STATE_HPP

#include "BasicInstruction.hpp"

class BasicInstructionSaveState : public BasicInstruction {
 private:
  uint64_t begin;
 public:
  BasicInstructionSaveState();
  BasicInstructionSaveState(uint64_t beginAddress, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionSaveState() = default;
  BasicInstructionSaveState(const BasicInstructionSaveState &from) = default;
  BasicInstructionSaveState &operator=(const BasicInstructionSaveState &from) = default;
  BasicInstructionSaveState(BasicInstructionSaveState &&) = default;
  BasicInstructionSaveState &operator=(BasicInstructionSaveState &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_SAVE_STATE_HPP