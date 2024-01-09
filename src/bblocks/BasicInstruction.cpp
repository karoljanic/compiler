#include "../../include/bblocks/BasicInstruction.hpp"

BasicInstruction::BasicInstruction() {}

BasicInstruction::BasicInstruction(std::shared_ptr<Hardware> hardware) : hardware(hardware) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstruction::expandToHardwareInstructions() const {
  return std::vector<std::pair<HardwareInstruction, std::string>>();
}

void BasicInstruction::print(std::ostream& out) const {
  out << "BasicInstruction";
}
