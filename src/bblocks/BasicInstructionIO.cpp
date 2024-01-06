#include "../../include/bblocks/BasicInstructionIO.hpp"

BasicInstructionRead::BasicInstructionRead() {}

BasicInstructionRead::BasicInstructionRead(const std::string& variableName) : leftRegister(variableName) {}

void BasicInstructionRead::print(std::ostream& out) const {
  out << "READ " << leftRegister;
}

BasicInstructionWrite::BasicInstructionWrite() {}

BasicInstructionWrite::BasicInstructionWrite(const std::string& variableName) : leftRegister(variableName) {}

void BasicInstructionWrite::print(std::ostream& out) const {
  out << "WRITE " << leftRegister;
}

BasicInstructionHalt::BasicInstructionHalt() {}

void BasicInstructionHalt::print(std::ostream& out) const {
  out << "HALT";
}
