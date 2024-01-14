#include "../../include/bblocks/BasicInstructionJumpRelative.hpp"

BasicInstructionJumpRelative::BasicInstructionJumpRelative() {}

BasicInstructionJumpRelative::BasicInstructionJumpRelative(std::string variableName, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), rightVariableName(variableName) {}

void BasicInstructionJumpRelative::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(std::make_pair(HardwareInstruction::JUMPR, rightVariableName));
}

void BasicInstructionJumpRelative::print(std::ostream& out) const {
  out << "JUMP AT ADDRESS FROM " << rightVariableName;
}