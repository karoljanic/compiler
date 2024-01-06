#include "../../include/bblocks/BasicInstructionStack.hpp"

BasicInstructionPush::BasicInstructionPush() {}

BasicInstructionPush::BasicInstructionPush(uint64_t beginAddress, const std::vector<std::string>& source)
    : begin(beginAddress), source(source) {}

void BasicInstructionPush::print(std::ostream& out) const {
  out << "PUSH " << begin << " ";
  for (auto& s : source) {
    out << s << " ";
  }
  out << std::endl;
}

BasicInstructionPop::BasicInstructionPop() {}

BasicInstructionPop::BasicInstructionPop(uint64_t beginAddress, const std::vector<std::string>& destination)
    : begin(beginAddress), destination(destination) {}

void BasicInstructionPop::print(std::ostream& out) const {
  out << "POP " << begin << " ";
  for (auto& s : destination) {
    out << s << " ";
  }
  out << std::endl;
}
