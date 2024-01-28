#include "../../include/bblocks/BasicInstructionSaveState.hpp"

BasicInstructionSaveState::BasicInstructionSaveState() {}

BasicInstructionSaveState::BasicInstructionSaveState(uint64_t beginAddress, std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware), begin(beginAddress) {}

void BasicInstructionSaveState::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::SAVE_STATE, std::to_string(begin)}});
}

void BasicInstructionSaveState::print(std::ostream &out) const {
  out << "SAVE STATE";
}