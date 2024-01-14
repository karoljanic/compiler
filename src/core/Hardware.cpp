#include "../../include/core/Hardware.hpp"

std::map<HardwareInstruction, HardwareInstructionDescriptor> Hardware::instructionMap = {
    {READ, {"READ", 100}}, {WRITE, {"WRITE", 100}}, {LOAD, {"LOAD", 50}},  {STORE, {"STORE", 50}}, {ADD, {"ADD", 5}},
    {SUB, {"SUB", 5}},     {GET, {"GET", 1}},       {PUT, {"PUT", 1}},     {RST, {"RST", 1}},      {INC, {"INC", 1}},
    {DEC, {"DEC", 1}},     {SHL, {"SHL", 1}},       {SHR, {"SHR", 1}},     {JUMP, {"JUMP", 1}},    {JPOS, {"JPOS", 1}},
    {JZERO, {"JZERO", 1}}, {STRK, {"STRK", 1}},     {JUMPR, {"JUMPR", 1}}, {HALT, {"HALT", 1}},    {LABEL, {"LABEL", 0}}};

std::map<HardwareRegister, HardwareRegisterDescriptor> Hardware::registerMap = {{RA, {"a", 1}}, {RB, {"b", 2}}, {RC, {"c", 3}},
                                                                                {RD, {"d", 4}}, {RE, {"e", 5}}, {RF, {"f", 6}},
                                                                                {RG, {"g", 7}}, {RH, {"h", 8}}};

HardwareRegister Hardware::accumulator = RA;

Hardware::Hardware() {}

std::string Hardware::getTempRegister() {
  tempsCounter++;
  return "temp#" + std::to_string(tempsCounter);
}

std::string Hardware::getLabel(const std::string& scopeName, const std::string& labelName, bool incrementCounter) {
  if (incrementCounter) {
    labelsCounter++;
  }

  return scopeName + "@" + labelName + "@" + std::to_string(labelsCounter);
}

std::string Hardware::getGlobalLabel(const std::string& scopeName, const std::string& labelName) {
  auto upperLabelName = labelName;

  return scopeName + "@" + labelName;
}

void Hardware::allocateArray(const std::string& arrayName, uint64_t size) {
#ifdef BEGIN_ON_POWER_OF_TWO_BOUNDARY
  memoryOffset = Hardware::nearestPowerOfTwo(memoryOffset);
#endif

  arrays[arrayName] = std::make_pair(memoryOffset, size);
  //std::cout << "Array " << arrayName << " allocated at " << memoryOffset << " with size " << size << std::endl;
  memoryOffset += size;
}

void Hardware::initializeStack() {
#ifdef BEGIN_ON_POWER_OF_TWO_BOUNDARY
  stackOffset = Hardware::nearestPowerOfTwo(memoryOffset);
  //std::cout << "Stack offset: " << stackOffset << std::endl;
#elif
  stackOffset = memoryOffset;
#endif
}

uint64_t Hardware::getArrayAddress(const std::string& arrayName) {
  return arrays[arrayName].first;
}

uint64_t Hardware::getStackOffset() {
  return stackOffset;
}

uint64_t Hardware::getFreeMemoryAddress() {
  stackOffset++;
  return stackOffset;
}

void Hardware::incrementStackOffset(uint64_t increment) {
  stackOffset += increment;
}

void Hardware::decrementStackOffset(uint64_t decrement) {
  stackOffset -= decrement;
}

uint64_t Hardware::nearestPowerOfTwo(uint64_t n) {
  if (n == 0) {
    return 0;
  }

  uint64_t result = 1;
  while (result < n) {
    result *= 2;
  }

  return result;
}
