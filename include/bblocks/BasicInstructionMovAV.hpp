#ifndef BASIC_INSTRUCTION_MOV_AV_HPP
#define BASIC_INSTRUCTION_MOV_AV_HPP

#include "BasicInstruction.hpp"

class BasicInstructionMovAV : public BasicInstruction {
 private:
  std::string arrayName;
  std::string indexRegister;
  std::string leftRegister;

 public:
  BasicInstructionMovAV();
  BasicInstructionMovAV(const std::string& arrayName, const std::string& indexVariableName, const std::string& variableName,
                        std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstructionMovAV() = default;
  BasicInstructionMovAV(const BasicInstructionMovAV& from) = default;
  BasicInstructionMovAV& operator=(const BasicInstructionMovAV& from) = default;
  BasicInstructionMovAV(BasicInstructionMovAV&&) = default;
  BasicInstructionMovAV& operator=(BasicInstructionMovAV&&) = default;

  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_MOV_AV_HPP