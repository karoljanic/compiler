#include "../../include/bblocks/BasicInstructionPush.hpp"

BasicInstructionPush::BasicInstructionPush() {}

BasicInstructionPush::BasicInstructionPush(uint64_t beginAddress, const std::vector<std::string>& source,
                                           std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), begin(beginAddress), source(source) {}

void BasicInstructionPush::expandToHardwareInstructions() {
  machineCode.clear();
}

void BasicInstructionPush::print(std::ostream& out) const {
  out << "PUSH " << begin << " ";
  for (auto& s : source) {
    out << s << " ";
  }
}
