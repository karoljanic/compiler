#include "../../include/bblocks/BasicInstructionRead.hpp"

BasicInstructionRead::BasicInstructionRead() {}

BasicInstructionRead::BasicInstructionRead(const std::string &variableName, std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware), leftRegister(variableName) {}

void BasicInstructionRead::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::READ, ""}});
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::PUT, leftRegister}});
}

void BasicInstructionRead::print(std::ostream &out) const {
  out << "READ " << leftRegister;
}