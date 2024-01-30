#include "../../include/bblocks/BasicInstructionWrite.hpp"

BasicInstructionWrite::BasicInstructionWrite() {}

BasicInstructionWrite::BasicInstructionWrite(const std::string &variableName, std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware, BasicInstructionType::WRITE), rightRegister(variableName) {}

void BasicInstructionWrite::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::GET, rightRegister}});
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::WRITE, ""}});
}

void BasicInstructionWrite::print(std::ostream &out) const {
  out << "WRITE " << rightRegister;
}