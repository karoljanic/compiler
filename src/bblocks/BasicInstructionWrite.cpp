#include "../../include/bblocks/BasicInstructionWrite.hpp"

BasicInstructionWrite::BasicInstructionWrite() {}

BasicInstructionWrite::BasicInstructionWrite(const std::string& variableName, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), rightRegister(variableName) {}

void BasicInstructionWrite::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(std::make_pair(HardwareInstruction::GET, rightRegister));
  machineCode.push_back(std::make_pair(HardwareInstruction::WRITE, ""));
}

void BasicInstructionWrite::print(std::ostream& out) const {
  out << "WRITE " << rightRegister;
}