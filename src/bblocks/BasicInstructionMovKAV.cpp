#include "../../include/bblocks/BasicInstructionMovKAV.hpp"

BasicInstructionMovKAV::BasicInstructionMovKAV() {}

BasicInstructionMovKAV::BasicInstructionMovKAV(const std::string& arrayName, uint64_t indexValue, const std::string& variableName,
                                               std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), arrayName(arrayName), index(indexValue), leftRegister(variableName) {}

void BasicInstructionMovKAV::expandToHardwareInstructions() {
  machineCode.clear();
  const auto& arrayBeginAddress =
      Utils::generateNumber(hardware->getArrayAddress(arrayName) + index, Hardware::registerMap[Hardware::accumulator].name);
  machineCode.insert(machineCode.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());

  machineCode.push_back({HardwareInstruction::LOAD, Hardware::registerMap[Hardware::accumulator].name});
  machineCode.push_back({HardwareInstruction::PUT, leftRegister});
}

void BasicInstructionMovKAV::print(std::ostream& out) const {
  out << "MOV " << arrayName << "[" << index << "]' TO " << leftRegister;
}