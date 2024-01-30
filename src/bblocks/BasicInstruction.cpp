#include "../../include/bblocks/BasicInstruction.hpp"

BasicInstruction::BasicInstruction() : type(BasicInstructionType::UNDEFINED) {}

BasicInstruction::BasicInstruction(std::shared_ptr<Hardware> hardwarePtr, BasicInstructionType instructionType) :
	hardware(hardwarePtr), type(instructionType) {}

const std::vector<MachineCodeType> &BasicInstruction::getMachineCode() const {
  return machineCode;
}

BasicInstructionType BasicInstruction::getType() const {
  return type;
}

size_t BasicInstruction::estimateMachineCodeSize() {
  return machineCode.size();
}

void BasicInstruction::expandToHardwareInstructions() {
  machineCode.clear();
}

void BasicInstruction::print(std::ostream &out) const {
  out << "BasicInstruction";
}
