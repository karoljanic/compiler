#include "../../include/bblocks/BasicInstructionIO.hpp"

BasicInstructionRead::BasicInstructionRead() {}

BasicInstructionRead::BasicInstructionRead(const std::string& variableName, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), leftRegister(variableName) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionRead::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back(std::make_pair(HardwareInstruction::READ, ""));
  instructions.push_back(std::make_pair(HardwareInstruction::PUT, leftRegister));

  return instructions;
}

void BasicInstructionRead::print(std::ostream& out) const {
  out << "READ " << leftRegister;
}

BasicInstructionWrite::BasicInstructionWrite() {}

BasicInstructionWrite::BasicInstructionWrite(const std::string& variableName, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), leftRegister(variableName) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionWrite::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back(std::make_pair(HardwareInstruction::GET, leftRegister));
  instructions.push_back(std::make_pair(HardwareInstruction::WRITE, ""));

  return instructions;
}

void BasicInstructionWrite::print(std::ostream& out) const {
  out << "WRITE " << leftRegister;
}

BasicInstructionHalt::BasicInstructionHalt() {}

BasicInstructionHalt::BasicInstructionHalt(std::shared_ptr<Hardware> hardware) : BasicInstruction(hardware) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionHalt::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back(std::make_pair(HardwareInstruction::HALT, ""));
  return instructions;
}

void BasicInstructionHalt::print(std::ostream& out) const {
  out << "HALT";
}
