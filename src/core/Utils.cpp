#include "../../include/core/Utils.hpp"

bool Utils::additionIsSafe(uint64_t a, uint64_t b) {
  return (std::numeric_limits<uint64_t>::max() - a) >= b;
}

bool Utils::multiplicationIsSafe(uint64_t a, uint64_t b) {
  return (std::numeric_limits<uint64_t>::max() / a) >= b;
}

std::vector<bool> Utils::getReversedBits(uint64_t number) {
  std::vector<bool> bits;
  while (number > 0) {
	bits.push_back(number % 2 == 1);
	number /= 2;
  }
  std::reverse(bits.begin(), bits.end());
  return bits;
}

std::vector<MachineCodeType> Utils::generateNumber(uint64_t number, const std::string &registerName) {
  std::vector<MachineCodeType> instructions;

  while (number > 0) {
	instructions.insert(instructions.begin(),
						MachineCodeType{{}, std::make_pair(HardwareInstruction::SHL, registerName)});

	if (number % 2 == 1) {
	  instructions.insert(instructions.begin(),
						  MachineCodeType{{}, std::make_pair(HardwareInstruction::INC, registerName)});
	}

	number /= 2;
  }

  if (!instructions.empty()) {
	instructions.pop_back();
  }

  instructions.insert(instructions.begin(),
					  MachineCodeType{{}, std::make_pair(HardwareInstruction::RST, registerName)});

  return instructions;
}

std::vector<MachineCodeType> Utils::generateVVAddition(const std::string &var1,
													   const std::string &var2,
													   const std::string &result) {
  std::vector<MachineCodeType> instructions;
  instructions.push_back({{}, {HardwareInstruction::GET, var1}});
  instructions.push_back({{}, {HardwareInstruction::ADD, var2}});
  instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  return instructions;
}

std::vector<MachineCodeType> Utils::generateVNAddition(const std::string &var,
													   uint64_t number,
													   const std::string &result) {
  std::vector<MachineCodeType> instructions;
  constexpr
  uint64_t maxIncNumberEconomy = 11;
  if (number <= maxIncNumberEconomy) {
	instructions.push_back({{}, {HardwareInstruction::GET, var}});
	instructions.push_back({{}, {HardwareInstruction::PUT, result}});
	for (uint64_t i = 0; i < number; i++) {
	  instructions.push_back({{}, {HardwareInstruction::INC, result}});
	}
  } else {
	const auto &generateNumber = Utils::generateNumber(number, Hardware::registerMap[Hardware::accumulator].name);
	instructions.insert(instructions.end(), generateNumber.begin(), generateNumber.end());
	instructions.push_back({{}, {HardwareInstruction::ADD, var}});
	instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  }
  return instructions;
}

std::vector<MachineCodeType> Utils::generateNNAddition(uint64_t number1, uint64_t number2,
													   const std::string &result,
													   std::shared_ptr<Hardware> hardware) {
  if (!Utils::additionIsSafe(number1, number2)) {
	const auto &temp = hardware->getTempRegister();

	std::vector<MachineCodeType> instructions;
	const auto &num1 = Utils::generateNumber(number1, Hardware::registerMap[Hardware::accumulator].name);
	instructions.insert(instructions.end(), num1.begin(), num1.end());
	const auto &num2 = Utils::generateNumber(number2, temp);
	instructions.insert(instructions.end(), num2.begin(), num2.end());
	instructions.push_back({{}, {HardwareInstruction::ADD, temp}});
	instructions.push_back({{}, {HardwareInstruction::PUT, result}});
	return instructions;
  }

  return Utils::generateNumber(number1 + number2, result);
}

std::vector<MachineCodeType> Utils::generateVVSubtraction(const std::string &var1,
														  const std::string &var2,
														  const std::string &result) {
  std::vector<MachineCodeType> instructions;
  instructions.push_back({{}, {HardwareInstruction::GET, var1}});
  instructions.push_back({{}, {HardwareInstruction::SUB, var2}});
  instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  return instructions;
}

std::vector<MachineCodeType> Utils::generateVNSubtraction(const std::string &var,
														  uint64_t number,
														  const std::string &result,
														  std::shared_ptr<Hardware> hardware) {
  std::vector<MachineCodeType> instructions;
  constexpr
  uint64_t maxIncNumberEconomy = 11;
  if (number <= maxIncNumberEconomy) {
	instructions.push_back({{}, {HardwareInstruction::GET, var}});
	instructions.push_back({{}, {HardwareInstruction::PUT, result}});
	for (uint64_t i = 0; i < number; i++) {
	  instructions.push_back({{}, {HardwareInstruction::DEC, result}});
	}
  } else {
	const auto &temp = hardware->getTempRegister();
	const auto &generateNumber = Utils::generateNumber(number, temp);
	instructions.insert(instructions.end(), generateNumber.begin(), generateNumber.end());
	instructions.push_back({{}, {HardwareInstruction::GET, var}});
	instructions.push_back({{}, {HardwareInstruction::SUB, temp}});
	instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  }
  return instructions;
}

std::vector<MachineCodeType> Utils::generateNVSubtraction(uint64_t number,
														  const std::string &var,
														  const std::string &result) {
  std::vector<MachineCodeType> instructions;
  const auto &generateNumber = Utils::generateNumber(number, Hardware::registerMap[Hardware::accumulator].name);
  instructions.insert(instructions.end(), generateNumber.begin(), generateNumber.end());
  instructions.push_back({{}, {HardwareInstruction::SUB, var}});
  instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  return instructions;
}

std::vector<MachineCodeType> Utils::generateNNSubtraction(uint64_t number1,
														  uint64_t number2,
														  const std::string &result) {
  if (number1 < number2) {
	return Utils::generateNumber(0, result);
  } else {
	return Utils::generateNumber(number1 - number2, result);
  }
}

std::vector<MachineCodeType> Utils::generateVVMultiplication(const std::string &var1,
															 const std::string &var2,
															 const std::string &result,
															 std::shared_ptr<Hardware> hardware) {
  const auto &mulBegin = hardware->getLabel("mul", "begin#", true);
  const auto &mulEven = hardware->getLabel("mul", "even#", true);
  const auto &mulEnd = hardware->getLabel("mul", "end#", true);

  const auto &tmp1 = hardware->getTempRegister();
  const auto &tmp2 = hardware->getTempRegister();

  std::vector<MachineCodeType> instructions;
  instructions.push_back({{}, {HardwareInstruction::RST, result}});
  instructions.push_back({{}, {HardwareInstruction::GET, var1}});
  instructions.push_back({{}, {HardwareInstruction::JZERO, mulEnd}});
  instructions.push_back({{}, {HardwareInstruction::PUT, tmp1}});
  instructions.push_back({{}, {HardwareInstruction::GET, var2}});
  instructions.push_back({{}, {HardwareInstruction::PUT, tmp2}});
  instructions.push_back({{{mulBegin, 0}}, {HardwareInstruction::GET, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::JZERO, mulEnd}});
  instructions.push_back({{}, {HardwareInstruction::SHR, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::SHL, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::SUB, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::JZERO, mulEven}});
  instructions.push_back({{}, {HardwareInstruction::GET, result}});
  instructions.push_back({{}, {HardwareInstruction::ADD, tmp1}});
  instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  instructions.push_back({{{mulEven, 0}}, {HardwareInstruction::SHL, tmp1}});
  instructions.push_back({{}, {HardwareInstruction::SHR, tmp2}});
  instructions.push_back({{{mulEnd, 1}}, {HardwareInstruction::JUMP, mulBegin}});

  return instructions;
}

std::vector<MachineCodeType> Utils::generateVNMultiplication(const std::string &var,
															 uint64_t number,
															 const std::string &result) {

  std::vector<MachineCodeType> instructions;
  if (number == 0) {
	instructions.push_back({{}, {HardwareInstruction::RST, result}});
  } else if (number == 1) {
	instructions.push_back({{}, {HardwareInstruction::GET, var}});
	instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  } else {
	const auto accumulator = Hardware::registerMap[Hardware::accumulator].name;
	instructions.push_back({{}, {HardwareInstruction::RST, accumulator}});
	for (const auto bit : getReversedBits(number)) {
	  instructions.push_back({{}, {HardwareInstruction::SHL, accumulator}});
	  if (bit) {
		instructions.push_back({{}, {HardwareInstruction::ADD, var}});
	  }
	}
	instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  }

  return instructions;
}

std::vector<MachineCodeType> Utils::generateNNMultiplication(uint64_t number1,
															 uint64_t number2,
															 const std::string &result,
															 std::shared_ptr<Hardware> hardware) {
  if (!Utils::multiplicationIsSafe(number1, number2)) {
	const auto &temp1 = hardware->getTempRegister();
	const auto &temp2 = hardware->getTempRegister();

	std::vector<MachineCodeType> instructions;
	const auto &num1 = Utils::generateNumber(number1, temp1);
	instructions.insert(instructions.end(), num1.begin(), num1.end());
	const auto &num2 = Utils::generateNumber(number2, temp2);
	instructions.insert(instructions.end(), num2.begin(), num2.end());
	const auto multiplication = Utils::generateVVMultiplication(temp1, temp2, result, hardware);
	instructions.insert(instructions.end(), multiplication.begin(), multiplication.end());

	return instructions;
  }

  return Utils::generateNumber(number1 * number2, result);
}

std::vector<MachineCodeType> Utils::generateVVDivision(const std::string &var1,
													   const std::string &var2,
													   const std::string &result,
													   std::shared_ptr<Hardware> hardware) {
  const auto &divPrepare = hardware->getLabel("div", "prepare#", true);
  const auto &divLoop = hardware->getLabel("div", "loop#", true);
  const auto &divEnd = hardware->getLabel("div", "end#", true);

  const auto &tmp1 = hardware->getTempRegister();
  const auto &tmp2 = hardware->getTempRegister();

  std::vector<MachineCodeType> instructions;
  instructions.push_back({{}, {HardwareInstruction::RST, result}});
  instructions.push_back({{}, {HardwareInstruction::GET, var2}});
  instructions.push_back({{}, {HardwareInstruction::JZERO, divEnd}});
  instructions.push_back({{}, {HardwareInstruction::PUT, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::GET, var1}});
  instructions.push_back({{}, {HardwareInstruction::PUT, tmp1}});
  instructions.push_back({{{divPrepare, 0}}, {HardwareInstruction::GET, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::SUB, tmp1}});
  instructions.push_back({{}, {HardwareInstruction::JPOS, divLoop}});
  instructions.push_back({{}, {HardwareInstruction::SHL, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::JUMP, divPrepare}});
  instructions.push_back({{{divLoop, 0}}, {HardwareInstruction::GET, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::SUB, var2}});
  instructions.push_back({{}, {HardwareInstruction::JZERO, divEnd}});
  instructions.push_back({{}, {HardwareInstruction::SHR, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::SHL, result}});
  instructions.push_back({{}, {HardwareInstruction::GET, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::SUB, tmp1}});
  instructions.push_back({{}, {HardwareInstruction::JPOS, divLoop}});
  instructions.push_back({{}, {HardwareInstruction::INC, result}});
  instructions.push_back({{}, {HardwareInstruction::GET, tmp1}});
  instructions.push_back({{}, {HardwareInstruction::SUB, tmp2}});
  instructions.push_back({{}, {HardwareInstruction::PUT, tmp1}});
  instructions.push_back({{{divEnd, 1}}, {HardwareInstruction::JUMP, divLoop}});

  return instructions;
}

std::vector<MachineCodeType> Utils::generateVNDivision(const std::string &var,
													   uint64_t number,
													   const std::string &result,
													   std::shared_ptr<Hardware> hardware) {
  std::vector<MachineCodeType> instructions;
  if (number == 0) {
	instructions.push_back({{}, {HardwareInstruction::RST, result}});
  } else if (number == 1) {
	instructions.push_back({{}, {HardwareInstruction::GET, var}});
	instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  } else {
	const auto bits = getReversedBits(number);
	size_t setBits = 0;
	for (const auto bit : bits) {
	  if (bit) {
		setBits++;
	  }
	}

	if (setBits == 1) {
	  instructions.push_back({{}, {HardwareInstruction::GET, var}});
	  instructions.push_back({{}, {HardwareInstruction::PUT, result}});
	  for (size_t i = 1; i < bits.size(); i++) {
		instructions.push_back({{}, {HardwareInstruction::SHR, result}});
	  }
	} else {
	  const auto &temp = hardware->getTempRegister();
	  const auto &num = Utils::generateNumber(number, temp);
	  instructions.insert(instructions.end(), num.begin(), num.end());
	  const auto div = Utils::generateVVDivision(var, temp, result, hardware);
	  instructions.insert(instructions.end(), div.begin(), div.end());
	}
  }

  return instructions;
}

std::vector<MachineCodeType> Utils::generateNVDivision(const std::string &var,
													   uint64_t number,
													   const std::string &result,
													   std::shared_ptr<Hardware> hardware) {
  std::vector<MachineCodeType> instructions;
  if (number == 0) {
	instructions.push_back({{}, {HardwareInstruction::RST, result}});
  } else {
	const auto temp = hardware->getTempRegister();
	const auto num = Utils::generateNumber(number, temp);
	instructions.insert(instructions.end(), num.begin(), num.end());
	const auto div = Utils::generateVVDivision(temp, var, result, hardware);
	instructions.insert(instructions.end(), div.begin(), div.end());
  }

  return instructions;
}

std::vector<MachineCodeType> Utils::generateNNDivision(uint64_t number1,
													   uint64_t number2,
													   const std::string &result) {
  if (number2 == 0) {
	return Utils::generateNumber(0, result);
  }

  return Utils::generateNumber(number1 / number2, result);
}

std::vector<MachineCodeType> Utils::generateVVModulo(const std::string &var1,
													 const std::string &var2,
													 const std::string &result,
													 std::shared_ptr<Hardware> hardware) {
  const auto &modPrepare = hardware->getLabel("mod", "prepare#", true);
  const auto &modLoop = hardware->getLabel("mod", "loop#", true);
  const auto &modEnd = hardware->getLabel("mod", "end#", true);

  const auto &tmp = hardware->getTempRegister();

  std::vector<MachineCodeType> instructions;
  instructions.push_back({{}, {HardwareInstruction::RST, result}});
  instructions.push_back({{}, {HardwareInstruction::GET, var2}});
  instructions.push_back({{}, {HardwareInstruction::JZERO, modEnd}});
  instructions.push_back({{}, {HardwareInstruction::PUT, tmp}});
  instructions.push_back({{}, {HardwareInstruction::GET, var1}});
  instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  instructions.push_back({{{modPrepare, 0}}, {HardwareInstruction::GET, tmp}});
  instructions.push_back({{}, {HardwareInstruction::SUB, result}});
  instructions.push_back({{}, {HardwareInstruction::JPOS, modLoop}});
  instructions.push_back({{}, {HardwareInstruction::SHL, tmp}});
  instructions.push_back({{}, {HardwareInstruction::JUMP, modPrepare}});
  instructions.push_back({{{modLoop, 0}}, {HardwareInstruction::GET, tmp}});
  instructions.push_back({{}, {HardwareInstruction::SUB, var2}});
  instructions.push_back({{}, {HardwareInstruction::JZERO, modEnd}});
  instructions.push_back({{}, {HardwareInstruction::SHR, tmp}});
  instructions.push_back({{}, {HardwareInstruction::GET, tmp}});
  instructions.push_back({{}, {HardwareInstruction::SUB, result}});
  instructions.push_back({{}, {HardwareInstruction::JPOS, modLoop}});
  instructions.push_back({{}, {HardwareInstruction::GET, result}});
  instructions.push_back({{}, {HardwareInstruction::SUB, tmp}});
  instructions.push_back({{}, {HardwareInstruction::PUT, result}});
  instructions.push_back({{{modEnd, 1}}, {HardwareInstruction::JUMP, modLoop}});

  return instructions;
}

std::vector<MachineCodeType> Utils::generateVNModulo(const std::string &var,
													 uint64_t number,
													 const std::string &result,
													 std::shared_ptr<Hardware> hardware) {
  std::vector<MachineCodeType> instructions;
  if (number == 0) {
	instructions.push_back({{}, {HardwareInstruction::RST, result}});
  } else if (number == 1) {
	instructions.push_back({{}, {HardwareInstruction::RST, result}});
  } else {
	const auto bits = getReversedBits(number);
	size_t setBits = 0;
	for (const auto bit : bits) {
	  if (bit) {
		setBits++;
	  }
	}

	if (setBits == 1) {
	  instructions.push_back({{}, {HardwareInstruction::GET, var}});
	  instructions.push_back({{}, {HardwareInstruction::PUT, result}});
	  for (size_t i = 1; i < bits.size(); i++) {
		instructions.push_back({{}, {HardwareInstruction::SHR, result}});
	  }
	  for (size_t i = 1; i < bits.size(); i++) {
		instructions.push_back({{}, {HardwareInstruction::SHL, result}});
	  }
	  instructions.push_back({{}, {HardwareInstruction::SUB, result}});
	  instructions.push_back({{}, {HardwareInstruction::PUT, result}});
	} else {
	  const auto &temp = hardware->getTempRegister();
	  const auto &num = Utils::generateNumber(number, temp);
	  instructions.insert(instructions.end(), num.begin(), num.end());
	  const auto div = Utils::generateVVModulo(var, temp, result, hardware);
	  instructions.insert(instructions.end(), div.begin(), div.end());
	}
  }

  return instructions;
}

std::vector<MachineCodeType> Utils::generateNVModulo(const std::string &var,
													 uint64_t number,
													 const std::string &result,
													 std::shared_ptr<Hardware> hardware) {
  std::vector<MachineCodeType> instructions;
  if(number == 0) {
	instructions.push_back({{}, {HardwareInstruction::RST, result}});
  } else {
	const auto temp = hardware->getTempRegister();
	const auto num = Utils::generateNumber(number, temp);
	instructions.insert(instructions.end(), num.begin(), num.end());
	const auto div = Utils::generateVVModulo(temp, var, result, hardware);
	instructions.insert(instructions.end(), div.begin(), div.end());
  }

  return instructions;
}

std::vector<MachineCodeType> Utils::generateNNModulo(uint64_t number1,
													 uint64_t number2,
													 const std::string &result) {
  if (number2 == 0) {
	return Utils::generateNumber(0, result);
  }

  return Utils::generateNumber(number1 % number2, result);
}
