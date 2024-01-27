#ifndef BASIC_INSTRUCTION_POP_HPP
#define BASIC_INSTRUCTION_POP_HPP

#include <cstdint>
#include <vector>

#include "BasicInstruction.hpp"

class BasicInstructionPop : public BasicInstruction {
 private:
  uint64_t begin;
  std::vector<std::string> destination;
  std::string ret;

 public:
  BasicInstructionPop();
  BasicInstructionPop(uint64_t beginAddress, const std::vector<std::string> &destination,
					  std::string returnAddress, std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionPop() = default;
  BasicInstructionPop(const BasicInstructionPop &from) = default;
  BasicInstructionPop &operator=(const BasicInstructionPop &from) = default;
  BasicInstructionPop(BasicInstructionPop &&) = default;
  BasicInstructionPop &operator=(BasicInstructionPop &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_POP_HPP