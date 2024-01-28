#include "../../include/bblocks/BasicInstruction.hpp"

BasicInstruction::BasicInstruction() {}

BasicInstruction::BasicInstruction(std::shared_ptr<Hardware> hardware) : hardware(hardware) {}

const std::vector<MachineCodeType> &BasicInstruction::getMachineCode() const {
  return machineCode;
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
