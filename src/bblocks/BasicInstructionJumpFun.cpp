#include "../../include/bblocks/BasicInstructionJumpFun.hpp"

BasicInstructionJumpFun::BasicInstructionJumpFun() {}

BasicInstructionJumpFun::BasicInstructionJumpFun(std::string functionName,
												 uint64_t retAddress,
												 std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware, BasicInstructionType::JUMP_FUN), name(functionName), returnAddress(retAddress) {}

void BasicInstructionJumpFun::expandToHardwareInstructions() {
  machineCode.clear();

  const auto label = hardware->getGlobalLabel(name, "begin#");
  const auto acc = Hardware::registerMap[RA].name;
  const auto retAddr = Hardware::registerMap[RB].name;
  const auto genAddr = Utils::generateNumber(returnAddress, retAddr);
  machineCode.insert(machineCode.end(), genAddr.begin(), genAddr.end());
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::STRK, acc}});
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::STORE, retAddr}});
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JUMP, label}});
}

void BasicInstructionJumpFun::print(std::ostream &out) const {
  out << "JUMP TO FUN " << name;
}