#ifndef BASIC_INSTRUCTION_RESTORE_STATE_HPP
#define BASIC_INSTRUCTION_RESTORE_STATE_HPP

#include "BasicInstruction.hpp"

class BasicInstructionRestoreState : public BasicInstruction {
 private:
  uint64_t begin;
 public:
  BasicInstructionRestoreState();
  BasicInstructionRestoreState(uint64_t beginAddress, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionRestoreState() = default;
  BasicInstructionRestoreState(const BasicInstructionRestoreState &from) = default;
  BasicInstructionRestoreState &operator=(const BasicInstructionRestoreState &from) = default;
  BasicInstructionRestoreState(BasicInstructionRestoreState &&) = default;
  BasicInstructionRestoreState &operator=(BasicInstructionRestoreState &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_RESTORE_STATE_HPP