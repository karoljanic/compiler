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

enum class BasicInstructionType {
  UNDEFINED,
  HALT,
  JUMP,
  JUMP_RELATIVE,
  JUMP_FUN,
  LABEL,
  MOV_VV,
  MOV_NV,
  MOV_VA,
  MOV_AV,
  MOV_VKA,
  MOV_KAV,
  MATH_VV,
  MATH_NV,
  MATH_VN,
  MATH_NN,
  COND_JUMP_VV,
  COND_JUMP_NV,
  COND_JUMP_VN,
  COND_JUMP_NN,
  COND_JUMP_VV_RELATIVE,
  COND_JUMP_NV_RELATIVE,
  COND_JUMP_VN_RELATIVE,
  COND_JUMP_NN_RELATIVE,
  POP,
  PUSH,
  READ,
  WRITE,
  SAVE,
  RESTORE
};

enum class BasicInstructionMathType { UNDEFINED, ADD, SUB, MUL, DIV, MOD };

enum class BasicInstructionConditionType { LESS, LESS_EQUAL, EQUAL, NOT_EQUAL };

class BasicInstruction {
 protected:
  std::vector<MachineCodeType> machineCode;
  std::shared_ptr<Hardware> hardware;
  BasicInstructionType type;

 public:
  BasicInstruction();
  BasicInstruction(std::shared_ptr<Hardware> hardwarePtr, BasicInstructionType instructionType);
  virtual ~BasicInstruction() = default;
  BasicInstruction(const BasicInstruction &from) = default;
  BasicInstruction &operator=(const BasicInstruction &from) = default;
  BasicInstruction(BasicInstruction &&) = default;
  BasicInstruction &operator=(BasicInstruction &&) = default;

  const std::vector<MachineCodeType> &getMachineCode() const;
  BasicInstructionType getType() const;

  size_t estimateMachineCodeSize();
  virtual void expandToHardwareInstructions();
  virtual void print(std::ostream &out) const;
};

#endif  // BASIC_INSTRUCTION_HPP
