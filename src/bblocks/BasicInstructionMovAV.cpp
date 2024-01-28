#include "../../include/bblocks/BasicInstructionMovAV.hpp"

BasicInstructionMovAV::BasicInstructionMovAV() {}

BasicInstructionMovAV::BasicInstructionMovAV(const std::string &arrayName, const std::string &indexVariableName,
											 const std::string &variableName, bool indirectArray,
											 std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware),
	  arrayName(arrayName),
	  indexRegister(indexVariableName),
	  leftRegister(variableName),
	  indirectArrayBeginAddress(indirectArray) {}

void BasicInstructionMovAV::expandToHardwareInstructions() {
  machineCode.clear();
  if (indirectArrayBeginAddress) {
	const auto &arrayBeginAddress = Utils::generateVVAddition(indexRegister, arrayName,
															  Hardware::registerMap[Hardware::accumulator].name);
	machineCode.insert(machineCode.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());
  } else {
	const auto &arrayBeginAddress = Utils::generateVNAddition(indexRegister, hardware->getArrayAddress(arrayName),
															  Hardware::registerMap[Hardware::accumulator].name);

	machineCode.insert(machineCode.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());
  }
  machineCode.push_back({{}, {HardwareInstruction::LOAD, Hardware::registerMap[Hardware::accumulator].name}});
  machineCode.push_back({{}, {HardwareInstruction::PUT, leftRegister}});
}

void BasicInstructionMovAV::print(std::ostream &out) const {
  out << "MOV " << arrayName << "[" << indexRegister << "] TO " << leftRegister;
}
