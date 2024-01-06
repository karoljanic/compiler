#include "../../include/core/Hardware.hpp"

std::map<HardwareInstruction, HardwareInstructionDescriptor> Hardware::instructionMap = {
    {READ, {"READ", 100}}, {WRITE, {"WRITE", 100}}, {LOAD, {"LOAD", 50}},  {STORE, {"STORE", 50}}, {ADD, {"ADD", 5}},
    {SUB, {"SUB", 5}},     {GET, {"GET", 1}},       {PUT, {"PUT", 1}},     {RST, {"RST", 1}},      {INC, {"INC", 1}},
    {DEC, {"DEC", 1}},     {SHL, {"SHL", 1}},       {SHR, {"SHR", 1}},     {JUMP, {"JUMP", 1}},    {JPOS, {"JPOS", 1}},
    {JZERO, {"JZERO", 1}}, {STRK, {"STRK", 1}},     {JUMPR, {"JUMPR", 1}}, {HALT, {"HALT", 1}}};

std::map<HardwareRegister, HardwareRegisterDescriptor> Hardware::registerMap = {{RA, {"RA", 1}}, {RB, {"RB", 2}}, {RC, {"RC", 3}},
                                                                                {RD, {"RD", 4}}, {RE, {"RE", 5}}, {RF, {"RF", 6}},
                                                                                {RG, {"RG", 7}}, {RH, {"RH", 8}}};

Hardware::Hardware() {}

void Hardware::allocateArray(const std::string& arrayName, uint64_t size) {
  arrays[arrayName] = std::make_pair(memoryOffset, size);
  memoryOffset += size;
}
