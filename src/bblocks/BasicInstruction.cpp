#include "../../include/bblocks/BasicInstruction.hpp"

BasicInstruction::BasicInstruction() : type(BasicInstructionType::UNDEFINED) {}

BasicInstruction::BasicInstruction(std::shared_ptr<Hardware> hardwarePtr, BasicInstructionType instructionType) :
	hardware(hardwarePtr), type(instructionType) {}

const std::vector<MachineCodeType> &BasicInstruction::getMachineCode() const {
  return machineCode;
}

BasicInstructionType BasicInstruction::getType() const {
  return type;
}

void BasicInstruction::replace(const std::map<std::string, std::string> &uselessTemporariesMapping) {
  for (auto &machineCodeLine : machineCode) {
	if (uselessTemporariesMapping.find(machineCodeLine.second.second) != uselessTemporariesMapping.end()) {
	  machineCodeLine.second.second = uselessTemporariesMapping.at(machineCodeLine.second.second);
	}
  }
}

size_t BasicInstruction::estimateMachineCodeSize() {
  return machineCode.size();
}

void BasicInstruction::expandToHardwareInstructions() {
  machineCode.clear();
}

void BasicInstruction::print(std::ostream &out) const {
  out << "BasicInstruction";
}
