#include "../../include/bblocks/BasicInstructionMovNV.hpp"

BasicInstructionMovNV::BasicInstructionMovNV() {}

BasicInstructionMovNV::BasicInstructionMovNV(uint64_t numValue, const std::string &variableName,
											 std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware), numValue(numValue), leftRegister(variableName) {}

void BasicInstructionMovNV::expandToHardwareInstructions() {
  machineCode.clear();
  const auto generateNumber = Utils::generateNumber(numValue, leftRegister);
  machineCode.insert(machineCode.end(), generateNumber.begin(), generateNumber.end());
}

void BasicInstructionMovNV::print(std::ostream &out) const {
  out << "MOV " << numValue << " TO " << leftRegister;
}