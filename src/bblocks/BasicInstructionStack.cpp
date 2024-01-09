#include "../../include/bblocks/BasicInstructionStack.hpp"

BasicInstructionPush::BasicInstructionPush() {}

BasicInstructionPush::BasicInstructionPush(uint64_t beginAddress, const std::vector<std::string>& source,
                                           std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), begin(beginAddress), source(source) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionPush::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> result;
  for (auto& s : source) {
    result.push_back(std::make_pair(LOAD, s));
    result.push_back(std::make_pair(PUT, "1"));
    result.push_back(std::make_pair(SHL, ""));
    result.push_back(std::make_pair(ADD, ""));
  }
  result.push_back(std::make_pair(STORE, std::to_string(begin)));
  return result;
}

void BasicInstructionPush::print(std::ostream& out) const {
  out << "PUSH " << begin << " ";
  for (auto& s : source) {
    out << s << " ";
  }
}

BasicInstructionPop::BasicInstructionPop() {}

BasicInstructionPop::BasicInstructionPop(uint64_t beginAddress, const std::vector<std::string>& destination,
                                         std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), begin(beginAddress), destination(destination) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionPop::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> result;
  for (auto& s : destination) {
    result.push_back(std::make_pair(LOAD, std::to_string(begin)));
    result.push_back(std::make_pair(PUT, "1"));
    result.push_back(std::make_pair(SHL, ""));
    result.push_back(std::make_pair(SUB, ""));
    result.push_back(std::make_pair(STORE, s));
  }
  return result;
}

void BasicInstructionPop::print(std::ostream& out) const {
  out << "POP " << begin << " ";
  for (auto& s : destination) {
    out << s << " ";
  }
}
