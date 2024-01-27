#include "../../include/bblocks/BasicInstructionJump.hpp"

BasicInstructionJump::BasicInstructionJump() {}

BasicInstructionJump::BasicInstructionJump(std::string label, std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware), label(label) {}

void BasicInstructionJump::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JUMP, label}});
}

void BasicInstructionJump::print(std::ostream &out) const {
  out << "JUMP TO " << label;
}