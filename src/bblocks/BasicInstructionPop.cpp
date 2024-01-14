#include "../../include/bblocks/BasicInstructionPop.hpp"

BasicInstructionPop::BasicInstructionPop() {}

BasicInstructionPop::BasicInstructionPop(uint64_t beginAddress, const std::vector<std::string>& destination,
                                         std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), begin(beginAddress), destination(destination) {}

void BasicInstructionPop::expandToHardwareInstructions() {
  machineCode.clear();
}

void BasicInstructionPop::print(std::ostream& out) const {
  out << "POP " << begin << " ";
  for (auto& s : destination) {
    out << s << " ";
  }
}