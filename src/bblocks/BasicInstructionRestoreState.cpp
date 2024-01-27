#include "../../include/bblocks/BasicInstructionRestoreState.hpp"

BasicInstructionRestoreState::BasicInstructionRestoreState() {}

BasicInstructionRestoreState::BasicInstructionRestoreState(uint64_t beginAddress, std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware), begin(beginAddress) {}

void BasicInstructionRestoreState::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::RESTORE_STATE, std::to_string(begin)}});

//  const std::vector<HardwareRegister> regs = {RB, RC, RD, RE, RF, RG, RH};
//  const auto addressReg = Hardware::registerMap[regs[regs.size() - 1]].name;
//  const auto genAddress = Utils::generateNumber(begin, addressReg);
//
//  machineCode.insert(machineCode.end(), genAddress.begin(), genAddress.end());
//  for (size_t i = 0; i < regs.size(); i++) {
//	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::LOAD, addressReg}});
//	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::DEC, addressReg}});
//	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::PUT, Hardware::registerMap[regs[i]].name}});
//  }
}

void BasicInstructionRestoreState::print(std::ostream &out) const {
  out << "RESTORE STATE";
}