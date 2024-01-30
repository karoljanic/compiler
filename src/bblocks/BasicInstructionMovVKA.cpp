#include "../../include/bblocks/BasicInstructionMovVKA.hpp"

BasicInstructionMovVKA::BasicInstructionMovVKA() {}

BasicInstructionMovVKA::BasicInstructionMovVKA(const std::string &variableName, const std::string &arrayName,
											   uint64_t indexValue, bool indirectArray,
											   std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware, BasicInstructionType::MOV_VKA),
	  rightRegister(variableName),
	  arrayName(arrayName),
	  index(indexValue),
	  indirectArrayBeginAddress(indirectArray) {}

void BasicInstructionMovVKA::expandToHardwareInstructions() {
  machineCode.clear();
  const auto &temp = hardware->getTempRegister();
  if (indirectArrayBeginAddress) {
	const auto &arrayBeginAddress = Utils::generateVNAddition(arrayName, index, temp);
	machineCode.insert(machineCode.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());
  } else {
	const auto &arrayBeginAddress =
		Utils::generateNumber(hardware->getArrayAddress(arrayName) + index, temp);
	machineCode.insert(machineCode.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());
  }
  machineCode.push_back({{}, {HardwareInstruction::GET, rightRegister}});
  machineCode.push_back({{}, {HardwareInstruction::STORE, temp}});
}

void BasicInstructionMovVKA::print(std::ostream &out) const {
  out << "MOV " << rightRegister << " TO " << arrayName << "[" << index << "]'";
}