#include "../../include/bblocks/BasicInstructionMovVA.hpp"

BasicInstructionMovVA::BasicInstructionMovVA() {}

BasicInstructionMovVA::BasicInstructionMovVA(const std::string &variableName, const std::string &arrayName,
											 const std::string &indexVariableName, std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware), rightRegister(variableName), arrayName(arrayName), indexRegister(indexVariableName) {}

void BasicInstructionMovVA::expandToHardwareInstructions() {
  machineCode.clear();
  const auto &temp = hardware->getTempRegister();
  const auto &arrayBeginAddress = Utils::generateVNAddition(indexRegister, hardware->getArrayAddress(arrayName),
															temp);

  machineCode.insert(machineCode.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());
  machineCode.push_back({{}, {HardwareInstruction::GET, rightRegister}});
  machineCode.push_back({{}, {HardwareInstruction::STORE, temp}});
}

void BasicInstructionMovVA::print(std::ostream &out) const {
  out << "MOV " << rightRegister << " TO " << arrayName << "[" << indexRegister << "]";
}