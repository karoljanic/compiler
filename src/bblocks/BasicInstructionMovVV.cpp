#include "../../include/bblocks/BasicInstructionMovVV.hpp"

BasicInstructionMovVV::BasicInstructionMovVV() {}

BasicInstructionMovVV::BasicInstructionMovVV(const std::string& rightVariableName, const std::string& leftVariableName,
                                             std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), leftRegister(leftVariableName), rightRegister(rightVariableName) {}

void BasicInstructionMovVV::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(std::make_pair(GET, rightRegister));
  machineCode.push_back(std::make_pair(PUT, leftRegister));
}

void BasicInstructionMovVV::print(std::ostream& out) const {
  out << "MOV " << rightRegister << " TO " << leftRegister;
}