#include "../../include/bblocks/BasicInstructionHalt.hpp"

BasicInstructionHalt::BasicInstructionHalt() {}

BasicInstructionHalt::BasicInstructionHalt(std::shared_ptr<Hardware> hardware) : BasicInstruction(hardware) {}

void BasicInstructionHalt::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::HALT, ""}});
}

void BasicInstructionHalt::print(std::ostream &out) const {
  out << "HALT";
}