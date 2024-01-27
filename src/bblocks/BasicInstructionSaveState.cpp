#include "../../include/bblocks/BasicInstructionSaveState.hpp"

BasicInstructionSaveState::BasicInstructionSaveState() {}

BasicInstructionSaveState::BasicInstructionSaveState(uint64_t beginAddress, std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware), begin(beginAddress) {}

void BasicInstructionSaveState::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::SAVE_STATE, std::to_string(begin)}});

//  const std::vector<HardwareRegister> regs = {RB, RC, RD, RE, RF, RG, RH};
//  const auto acc = Hardware::registerMap[RA].name;
//
//  for (size_t i = 0; i < regs.size(); i++) {
//	const auto currReg = Hardware::registerMap[regs[i]].name;
//	const auto genAddress = Utils::generateNumber(begin, currReg);
//	begin++;
//	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::GET, currReg}});
//	machineCode.insert(machineCode.end(), genAddress.begin(), genAddress.end());
//	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::STORE, currReg}});
//	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::PUT, currReg}});
//  }
}

void BasicInstructionSaveState::print(std::ostream &out) const {
  out << "SAVE STATE";
}