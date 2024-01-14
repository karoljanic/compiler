#include "../../include/bblocks/BasicInstructionLabel.hpp"

BasicInstructionLabel::BasicInstructionLabel() {}

BasicInstructionLabel::BasicInstructionLabel(std::string label, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), label(label) {}

void BasicInstructionLabel::expandToHardwareInstructions() {
  machineCode.clear();
  machineCode.push_back(std::make_pair(HardwareInstruction::LABEL, label));
}

void BasicInstructionLabel::print(std::ostream& out) const {
  if (label.find("begin#") != std::string::npos) {
    out << std::endl;
  }
  out << "LABEL " << label;
  if (label.find("end#") != std::string::npos) {
    out << std::endl;
  }
}