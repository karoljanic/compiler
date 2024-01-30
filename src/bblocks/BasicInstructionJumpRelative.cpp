#include "../../include/bblocks/BasicInstructionJumpRelative.hpp"

BasicInstructionJumpRelative::BasicInstructionJumpRelative() {}

BasicInstructionJumpRelative::BasicInstructionJumpRelative(std::string variableName, std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware, BasicInstructionType::JUMP_RELATIVE), rightVariableName(variableName) {}

void BasicInstructionJumpRelative::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::RST, Hardware::registerMap[RA].name}});
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::INC, Hardware::registerMap[RA].name}});
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::SHL, Hardware::registerMap[RA].name}});
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::INC, Hardware::registerMap[RA].name}});
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::ADD, rightVariableName}});
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JUMPR, Hardware::registerMap[RA].name}});
}

void BasicInstructionJumpRelative::print(std::ostream &out) const {
  out << "JUMP AT ADDRESS FROM " << rightVariableName;
}