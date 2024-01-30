#include "../../include/bblocks/BasicInstructionRestoreState.hpp"

BasicInstructionRestoreState::BasicInstructionRestoreState() {}

BasicInstructionRestoreState::BasicInstructionRestoreState(uint64_t beginAddress, std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware, BasicInstructionType::RESTORE), begin(beginAddress) {}

void BasicInstructionRestoreState::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::RESTORE_STATE, std::to_string(begin)}});
}

void BasicInstructionRestoreState::print(std::ostream &out) const {
  out << "RESTORE STATE";
}