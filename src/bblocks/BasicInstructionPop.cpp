#include "../../include/bblocks/BasicInstructionPop.hpp"

BasicInstructionPop::BasicInstructionPop() {}

BasicInstructionPop::BasicInstructionPop(uint64_t beginAddress, const std::vector<std::string> &destination,
										 std::string returnAddress, std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware, BasicInstructionType::POP),
	  begin(beginAddress),
	  destination(destination),
	  ret(returnAddress) {}

void BasicInstructionPop::expandToHardwareInstructions() {
  machineCode.clear();

  for (size_t i = 0; i < destination.size(); i++) {
	const auto generateAddress = Utils::generateNumber(begin + i, destination[i]);
	machineCode.insert(machineCode.end(), generateAddress.begin(), generateAddress.end());
	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::LOAD, destination[i]}});
	machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::PUT, destination[i]}});
  }
  const auto generateAddress = Utils::generateNumber(begin + destination.size(), ret);
  machineCode.insert(machineCode.end(), generateAddress.begin(), generateAddress.end());
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::LOAD, ret}});
  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::PUT, ret}});
}

void BasicInstructionPop::print(std::ostream &out) const {
  out << "POP " << begin << " ";
  for (auto &s : destination) {
	out << s << " ";
  }
}