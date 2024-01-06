#include "../../include/bblocks/BasicInstructionMov.hpp"

BasicInstructionMovVarToVar::BasicInstructionMovVarToVar() {}

BasicInstructionMovVarToVar::BasicInstructionMovVarToVar(const std::string& variable1Name, const std::string& variable2Name)
    : leftRegister(variable1Name), rightRegister(variable2Name) {}

void BasicInstructionMovVarToVar::print(std::ostream& out) const {
  out << "MOV " << leftRegister << " TO " << rightRegister;
}

BasicInstructionMovVarToArr::BasicInstructionMovVarToArr() {}

BasicInstructionMovVarToArr::BasicInstructionMovVarToArr(const std::string& variableName, const std::string& arrayName,
                                                         const std::string& indexVariableName)
    : leftRegister(variableName), arrayName(arrayName), indexRegister(indexVariableName) {}

void BasicInstructionMovVarToArr::print(std::ostream& out) const {
  out << "MOV " << leftRegister << " TO " << arrayName << "[" << indexRegister << "]";
}

BasicInstructionMovVarToKnownArr::BasicInstructionMovVarToKnownArr() {}

BasicInstructionMovVarToKnownArr::BasicInstructionMovVarToKnownArr(const std::string& variableName, const std::string& arrayName,
                                                                   uint64_t indexValue)
    : leftRegister(variableName), arrayName(arrayName), index(indexValue) {}

void BasicInstructionMovVarToKnownArr::print(std::ostream& out) const {
  out << "MOV " << leftRegister << " TO " << arrayName << "[" << index << "]'";
}

BasicInstructionMovArrToVar::BasicInstructionMovArrToVar() {}

BasicInstructionMovArrToVar::BasicInstructionMovArrToVar(const std::string& arrayName, const std::string& indexVariableName,
                                                         const std::string& variableName)
    : arrayName(arrayName), indexRegister(indexVariableName), rightRegister(variableName) {}

void BasicInstructionMovArrToVar::print(std::ostream& out) const {
  out << "MOV " << arrayName << "[" << indexRegister << "] TO " << rightRegister;
}

BasicInstructionMovKnownArrToVar::BasicInstructionMovKnownArrToVar() {}

BasicInstructionMovKnownArrToVar::BasicInstructionMovKnownArrToVar(const std::string& arrayName, uint64_t indexValue,
                                                                   const std::string& variableName)
    : arrayName(arrayName), index(indexValue), rightRegister(variableName) {}

void BasicInstructionMovKnownArrToVar::print(std::ostream& out) const {
  out << "MOV " << arrayName << "[" << index << "]' TO " << rightRegister;
}

BasicInstructionMovVarToMem::BasicInstructionMovVarToMem() {}

BasicInstructionMovVarToMem::BasicInstructionMovVarToMem(const std::string& variableName, const std::string& memoryAddress)
    : leftRegister(variableName), memoryAddress(memoryAddress) {}

void BasicInstructionMovVarToMem::print(std::ostream& out) const {
  out << "mov " << leftRegister << " TO [" << memoryAddress << "]";
}

BasicInstructionMovMemToVar::BasicInstructionMovMemToVar() {}

BasicInstructionMovMemToVar::BasicInstructionMovMemToVar(const std::string& memoryAddress, const std::string& variableName)
    : memoryAddress(memoryAddress), rightRegister(variableName) {}

void BasicInstructionMovMemToVar::print(std::ostream& out) const {
  out << "mov [" << memoryAddress << "] TO " << rightRegister;
}

BasicInstructionMovNumToVar::BasicInstructionMovNumToVar() {}

BasicInstructionMovNumToVar::BasicInstructionMovNumToVar(uint64_t numValue, const std::string& variableName)
    : numValue(numValue), leftRegister(variableName) {}

void BasicInstructionMovNumToVar::print(std::ostream& out) const {
  out << "MOV " << numValue << " TO " << leftRegister;
}
