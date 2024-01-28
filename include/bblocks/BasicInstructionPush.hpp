#ifndef BASIC_INSTRUCTION_PUSH_HPP
#define BASIC_INSTRUCTION_PUSH_HPP

#include <cstdint>
#include <vector>

#include "BasicInstruction.hpp"

class BasicInstructionPush : public BasicInstruction {
 private:
  uint64_t begin;
  std::vector<std::string> source;

 public:
  BasicInstructionPush();
  BasicInstructionPush(uint64_t beginAddress,
					   const std::vector<std::string> &source,
					   std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionPush() = default;
  BasicInstructionPush(const BasicInstructionPush &from) = default;
  BasicInstructionPush &operator=(const BasicInstructionPush &from) = default;
  BasicInstructionPush(BasicInstructionPush &&) = default;
  BasicInstructionPush &operator=(BasicInstructionPush &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_PUSH_HPP