#include "../../include/bblocks/BasicInstruction.hpp"

BasicInstruction::BasicInstruction() {}

BasicInstruction::BasicInstruction(std::shared_ptr<Hardware> hardware) : hardware(hardware) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstruction::getMachineCode() const {
  std::vector<std::pair<HardwareInstruction, std::string>> result;
  for (auto& [instruction, argument] : machineCode) {
    result.push_back(std::make_pair(instruction, argument));
  }
  //  for (auto& [instruction, argument] : machineCode) {
  //    result.push_back(std::make_pair(instruction, hardware->getVariableRegister(argument)));
  //  }
  return result;
}

void BasicInstruction::expandToHardwareInstructions() {
  machineCode.clear();
}

void BasicInstruction::print(std::ostream& out) const {
  out << "BasicInstruction";
}
