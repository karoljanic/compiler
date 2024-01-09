#include "../../include/bblocks/BasicInstructionMov.hpp"

BasicInstructionMovVarToVar::BasicInstructionMovVarToVar() {}

BasicInstructionMovVarToVar::BasicInstructionMovVarToVar(const std::string& variable1Name, const std::string& variable2Name,
                                                         std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), leftRegister(variable1Name), rightRegister(variable2Name) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMovVarToVar::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back(std::make_pair(GET, leftRegister));
  instructions.push_back(std::make_pair(PUT, rightRegister));
  return instructions;
}

void BasicInstructionMovVarToVar::print(std::ostream& out) const {
  out << "MOV " << leftRegister << " TO " << rightRegister;
}

BasicInstructionMovVarToArr::BasicInstructionMovVarToArr() {}

BasicInstructionMovVarToArr::BasicInstructionMovVarToArr(const std::string& variableName, const std::string& arrayName,
                                                         const std::string& indexVariableName, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), leftRegister(variableName), arrayName(arrayName), indexRegister(indexVariableName) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMovVarToArr::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  const auto& arrayBeginAddress = Utils::generateVarNumberAddition(indexRegister, hardware->getArrayAddress(arrayName),
                                                                   Hardware::registerMap[Hardware::accumulator].name);
  instructions.insert(instructions.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());
  instructions.push_back({HardwareInstruction::STORE, leftRegister});

  return instructions;
}

void BasicInstructionMovVarToArr::print(std::ostream& out) const {
  out << "MOV " << leftRegister << " TO " << arrayName << "[" << indexRegister << "]";
}

BasicInstructionMovVarToKnownArr::BasicInstructionMovVarToKnownArr() {}

BasicInstructionMovVarToKnownArr::BasicInstructionMovVarToKnownArr(const std::string& variableName, const std::string& arrayName,
                                                                   uint64_t indexValue, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), leftRegister(variableName), arrayName(arrayName), index(indexValue) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMovVarToKnownArr::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  const auto& arrayBeginAddress =
      Utils::generateNumber(hardware->getArrayAddress(arrayName) + index, Hardware::registerMap[Hardware::accumulator].name);
  instructions.insert(instructions.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());

  instructions.push_back({HardwareInstruction::STORE, leftRegister});

  return instructions;
}

void BasicInstructionMovVarToKnownArr::print(std::ostream& out) const {
  out << "MOV " << leftRegister << " TO " << arrayName << "[" << index << "]'";
}

BasicInstructionMovArrToVar::BasicInstructionMovArrToVar() {}

BasicInstructionMovArrToVar::BasicInstructionMovArrToVar(const std::string& arrayName, const std::string& indexVariableName,
                                                         const std::string& variableName, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), arrayName(arrayName), indexRegister(indexVariableName), rightRegister(variableName) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMovArrToVar::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  const auto& arrayBeginAddress = Utils::generateVarNumberAddition(indexRegister, hardware->getArrayAddress(arrayName),
                                                                   Hardware::registerMap[Hardware::accumulator].name);
  instructions.insert(instructions.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());

  instructions.push_back({HardwareInstruction::LOAD, Hardware::registerMap[Hardware::accumulator].name});
  instructions.push_back({HardwareInstruction::PUT, rightRegister});

  return instructions;
}

void BasicInstructionMovArrToVar::print(std::ostream& out) const {
  out << "MOV " << arrayName << "[" << indexRegister << "] TO " << rightRegister;
}

BasicInstructionMovKnownArrToVar::BasicInstructionMovKnownArrToVar() {}

BasicInstructionMovKnownArrToVar::BasicInstructionMovKnownArrToVar(const std::string& arrayName, uint64_t indexValue,
                                                                   const std::string& variableName,
                                                                   std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), arrayName(arrayName), index(indexValue), rightRegister(variableName) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMovKnownArrToVar::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  const auto& arrayBeginAddress =
      Utils::generateNumber(hardware->getArrayAddress(arrayName) + index, Hardware::registerMap[Hardware::accumulator].name);
  instructions.insert(instructions.end(), arrayBeginAddress.begin(), arrayBeginAddress.end());

  instructions.push_back({HardwareInstruction::LOAD, Hardware::registerMap[Hardware::accumulator].name});
  instructions.push_back({HardwareInstruction::PUT, rightRegister});

  return instructions;
}

void BasicInstructionMovKnownArrToVar::print(std::ostream& out) const {
  out << "MOV " << arrayName << "[" << index << "]' TO " << rightRegister;
}

//BasicInstructionMovVarToMem::BasicInstructionMovVarToMem() {}
//
//BasicInstructionMovVarToMem::BasicInstructionMovVarToMem(const std::string& variableName, const std::string& memoryAddress,
//                                                         std::shared_ptr<Hardware> hardware)
//    : BasicInstruction(hardware), leftRegister(variableName), memoryAddress(memoryAddress) {}
//
//std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMovVarToMem::expandToHardwareInstructions() const {
//  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
//  instructions.push_back(std::make_pair(GET, leftRegister));
//  instructions.push_back(std::make_pair(STORE, memoryAddress));
//  return instructions;
//}
//
//void BasicInstructionMovVarToMem::print(std::ostream& out) const {
//  out << "MOV " << leftRegister << " TO [" << memoryAddress << "]";
//}
//
//BasicInstructionMovMemToVar::BasicInstructionMovMemToVar() {}
//
//BasicInstructionMovMemToVar::BasicInstructionMovMemToVar(const std::string& memoryAddress, const std::string& variableName,
//                                                         std::shared_ptr<Hardware> hardware)
//    : BasicInstruction(hardware), memoryAddress(memoryAddress), rightRegister(variableName) {}
//
//std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMovMemToVar::expandToHardwareInstructions() const {
//  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
//  instructions.push_back(std::make_pair(LOAD, memoryAddress));
//  instructions.push_back(std::make_pair(STORE, rightRegister));
//  return instructions;
//}
//
//void BasicInstructionMovMemToVar::print(std::ostream& out) const {
//  out << "MOV [" << memoryAddress << "] TO " << rightRegister;
//}

BasicInstructionMovNumToVar::BasicInstructionMovNumToVar() {}

BasicInstructionMovNumToVar::BasicInstructionMovNumToVar(uint64_t numValue, const std::string& variableName,
                                                         std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), numValue(numValue), leftRegister(variableName) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMovNumToVar::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions = Utils::generateNumber(numValue, leftRegister);
  return instructions;
}

void BasicInstructionMovNumToVar::print(std::ostream& out) const {
  out << "MOV " << numValue << " TO " << leftRegister;
}
