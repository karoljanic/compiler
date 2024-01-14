#include "../../include/bblocks/BasicInstructionMovVKA.hpp"

BasicInstructionMovVKA::BasicInstructionMovVKA() {}

BasicInstructionMovVKA::BasicInstructionMovVKA(const std::string& variableName, const std::string& arrayName, uint64_t indexValue,
                                               std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), rightRegister(variableName), arrayName(arrayName), index(indexValue) {}

void BasicInstructionMovVKA::expandToHardwareInstructions() {
  machineCode.clear();
  const auto& arrayBeginAddress =
      Utils::generateNumber(hardware->getArrayAddress(arrayName) + index, Hardware::registerMap[Hardware::accumulator].name);
  machineCode.insert(machineCode.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());
  machineCode.push_back({HardwareInstruction::STORE, rightRegister});
}

void BasicInstructionMovVKA::print(std::ostream& out) const {
  out << "MOV " << rightRegister << " TO " << arrayName << "[" << index << "]'";
}