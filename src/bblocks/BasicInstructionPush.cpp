#include "../../include/bblocks/BasicInstructionPush.hpp"

BasicInstructionPush::BasicInstructionPush() {}

BasicInstructionPush::BasicInstructionPush(uint64_t beginAddress, const std::vector<std::string> &source,
										   std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware), begin(beginAddress), source(source) {}

void BasicInstructionPush::expandToHardwareInstructions() {
  machineCode.clear();

  for (size_t i = 0; i < source.size(); i++) {
	const auto generateAddress = Utils::generateNumber(begin + i, source[i]);
	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::GET, source[i]}});
	machineCode.insert(machineCode.end(), generateAddress.begin(), generateAddress.end());
	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::STORE, source[i]}});
	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::PUT, source[i]}});
  }
}

void BasicInstructionPush::print(std::ostream &out) const {
  out << "PUSH " << begin << " ";
  for (auto &s : source) {
	out << s << " ";
  }
}
