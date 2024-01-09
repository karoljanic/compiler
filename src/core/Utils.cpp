#include "../../include/core/Utils.hpp"

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateNumber(uint64_t number, const std::string& registerName) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;

  while (number > 0) {
    instructions.insert(instructions.begin(), std::make_pair(HardwareInstruction::SHL, registerName));

    if (number % 2 == 1) {
      instructions.insert(instructions.begin(), std::make_pair(HardwareInstruction::INC, registerName));
    }

    number /= 2;
  }

  if (!instructions.empty()) {
    instructions.pop_back();
  }

  instructions.insert(instructions.begin(), std::make_pair(HardwareInstruction::RST, registerName));

  return instructions;
}

bool Utils::additionIsSafe(uint64_t a, uint64_t b) {
  return (std::numeric_limits<uint64_t>::max() - a) >= b;
}

bool Utils::multiplicationIsSafe(uint64_t a, uint64_t b) {
  return (std::numeric_limits<uint64_t>::max() / a) >= b;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateVarNumberAddition(const std::string& var, uint64_t number,
                                                                                          const std::string& result) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  constexpr uint64_t maxIncNumberEconomy = 11;
  if (number <= maxIncNumberEconomy) {
    instructions.push_back({HardwareInstruction::GET, var});
    instructions.push_back({HardwareInstruction::PUT, result});
    for (uint64_t i = 0; i < number; i++) {
      instructions.push_back({HardwareInstruction::INC, result});
    }
  }
  else {
    const auto& generateNumber = Utils::generateNumber(number, Hardware::registerMap[Hardware::accumulator].name);
    instructions.insert(instructions.begin(), generateNumber.begin(), generateNumber.end());
    instructions.push_back({HardwareInstruction::ADD, var});
    instructions.push_back({HardwareInstruction::PUT, result});
  }
  return instructions;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateVarNumberSubtraction(const std::string& var,
                                                                                             uint64_t number,
                                                                                             const std::string& result,
                                                                                             std::shared_ptr<Hardware> hardware) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  constexpr uint64_t maxIncNumberEconomy = 11;
  if (number <= maxIncNumberEconomy) {
    instructions.push_back({HardwareInstruction::GET, var});
    instructions.push_back({HardwareInstruction::PUT, result});
    for (uint64_t i = 0; i < number; i++) {
      instructions.push_back({HardwareInstruction::DEC, result});
    }
  }
  else {
    const auto& temp = hardware->getTempRegister();
    const auto& generateNumber = Utils::generateNumber(number, temp);
    instructions.insert(instructions.begin(), generateNumber.begin(), generateNumber.end());
    instructions.push_back({HardwareInstruction::GET, var});
    instructions.push_back({HardwareInstruction::SUB, temp});
    instructions.push_back({HardwareInstruction::PUT, result});
  }
  return instructions;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateNumberVarSubtraction(uint64_t number,
                                                                                             const std::string& var,
                                                                                             const std::string& result) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  const auto& generateNumber = Utils::generateNumber(number, Hardware::registerMap[Hardware::accumulator].name);
  instructions.insert(instructions.begin(), generateNumber.begin(), generateNumber.end());
  instructions.push_back({HardwareInstruction::SUB, var});
  instructions.push_back({HardwareInstruction::PUT, result});
  return instructions;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateVarVarMultiplication(const std::string& var1,
                                                                                             const std::string& var2,
                                                                                             const std::string& result,
                                                                                             std::shared_ptr<Hardware> hardware) {
  const auto& mulBegin = hardware->getLabel("mul", "begin#", true);
  const auto& mulEven = hardware->getLabel("mul", "even#", true);
  const auto& mulEnd = hardware->getLabel("mul", "end#", true);

  const auto& tmp1 = hardware->getTempRegister();
  const auto& tmp2 = hardware->getTempRegister();

  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back({HardwareInstruction::GET, var1});
  instructions.push_back({HardwareInstruction::JZERO, mulEnd});
  instructions.push_back({HardwareInstruction::RST, tmp1});
  instructions.push_back({HardwareInstruction::LABEL, mulBegin});
  instructions.push_back({HardwareInstruction::GET, var2});
  instructions.push_back({HardwareInstruction::JZERO, mulEnd});
  instructions.push_back({HardwareInstruction::SHR, var2});
  instructions.push_back({HardwareInstruction::SHL, var2});
  instructions.push_back({HardwareInstruction::SUB, var2});
  instructions.push_back({HardwareInstruction::JZERO, mulEven});
  instructions.push_back({HardwareInstruction::PUT, tmp2});
  instructions.push_back({HardwareInstruction::GET, tmp1});
  instructions.push_back({HardwareInstruction::ADD, var1});
  instructions.push_back({HardwareInstruction::GET, tmp2});
  instructions.push_back({HardwareInstruction::PUT, tmp1});
  instructions.push_back({HardwareInstruction::LABEL, mulEven});
  instructions.push_back({HardwareInstruction::SHL, var1});
  instructions.push_back({HardwareInstruction::SHR, var2});
  instructions.push_back({HardwareInstruction::JUMP, mulBegin});
  instructions.push_back({HardwareInstruction::LABEL, mulEnd});
  instructions.push_back({HardwareInstruction::PUT, result});

  return instructions;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateVarNumberMultiplication(const std::string& var,
                                                                                                uint64_t number) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back({HardwareInstruction::RST, Hardware::registerMap[Hardware::accumulator].name});

  return instructions;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateVarVarDivision(const std::string& var1,
                                                                                       const std::string& var2) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back({HardwareInstruction::RST, Hardware::registerMap[Hardware::accumulator].name});

  return instructions;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateVarNumberDivision(const std::string& var,
                                                                                          uint64_t number) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back({HardwareInstruction::RST, Hardware::registerMap[Hardware::accumulator].name});

  return instructions;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateNumberVarDivision(const std::string& var,
                                                                                          uint64_t number) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back({HardwareInstruction::RST, Hardware::registerMap[Hardware::accumulator].name});

  return instructions;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateVarVarModulo(const std::string& var1,
                                                                                     const std::string& var2) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back({HardwareInstruction::RST, Hardware::registerMap[Hardware::accumulator].name});

  return instructions;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateVarNumberModulo(const std::string& var, uint64_t number) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back({HardwareInstruction::RST, Hardware::registerMap[Hardware::accumulator].name});

  return instructions;
}

std::vector<std::pair<HardwareInstruction, std::string>> Utils::generateNumberVarModulo(const std::string& var, uint64_t number) {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  instructions.push_back({HardwareInstruction::RST, Hardware::registerMap[Hardware::accumulator].name});

  return instructions;
}
