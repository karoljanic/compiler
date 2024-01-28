#ifndef BASIC_INSTRUCTION_MOV_KAV_HPP
#define BASIC_INSTRUCTION_MOV_KAV_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMovKAV : public BasicInstruction {
 private:
  std::string arrayName;
  uint64_t index;
  std::string leftRegister;
  bool indirectArrayBeginAddress;

 public:
  BasicInstructionMovKAV();
  BasicInstructionMovKAV(const std::string &arrayName, uint64_t indexValue,
						 const std::string &variableName, bool indirectArray,
						 std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMovKAV() = default;
  BasicInstructionMovKAV(const BasicInstructionMovKAV &from) = default;
  BasicInstructionMovKAV &operator=(const BasicInstructionMovKAV &from) = default;
  BasicInstructionMovKAV(BasicInstructionMovKAV &&) = default;
  BasicInstructionMovKAV &operator=(BasicInstructionMovKAV &&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_MOV_KAV_HPP