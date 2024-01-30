#include "../../include/bblocks/BasicInstructionMathNV.hpp"

BasicInstructionMathNV::BasicInstructionMathNV() {}

BasicInstructionMathNV::BasicInstructionMathNV(BasicInstructionMathType operationType, uint64_t leftOperand,
											   const std::string &rightOperand, const std::string &result,
											   std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware, BasicInstructionType::MATH_NV),
	  operationType(operationType),
	  leftOperand(leftOperand),
	  rightOperand(rightOperand),
	  result(result) {}

void BasicInstructionMathNV::expandToHardwareInstructions() {
  machineCode.clear();
  switch (operationType) {
	case BasicInstructionMathType::ADD: {
	  const auto &instructions = Utils::generateVNAddition(rightOperand, leftOperand, result);
	  machineCode.insert(machineCode.end(), instructions.begin(), instructions.end());
	  break;
	}
	case BasicInstructionMathType::SUB: {
	  const auto &instructions = Utils::generateNVSubtraction(leftOperand, rightOperand, result);
	  machineCode.insert(machineCode.end(), instructions.begin(), instructions.end());
	  break;
	}
	case BasicInstructionMathType::MUL: {
	  const auto &instructions = Utils::generateVNMultiplication(rightOperand, leftOperand, result);
	  machineCode.insert(machineCode.end(), instructions.begin(), instructions.end());
	  break;
	}
	case BasicInstructionMathType::DIV: {
	  const auto &instructions = Utils::generateNVDivision(rightOperand, leftOperand, result, hardware);
	  machineCode.insert(machineCode.end(), instructions.begin(), instructions.end());
	  break;
	}
	case BasicInstructionMathType::MOD: {
	  const auto &instructions = Utils::generateNVModulo(rightOperand, leftOperand, result, hardware);
	  machineCode.insert(machineCode.end(), instructions.begin(), instructions.end());
	  break;
	}
	case BasicInstructionMathType::UNDEFINED: {
	  break;
	}
  }
}

void BasicInstructionMathNV::print(std::ostream &out) const {
  out << result << " = " << leftOperand << " ";
  switch (operationType) {
	case BasicInstructionMathType::ADD:out << "+";
	  break;
	case BasicInstructionMathType::SUB:out << "-";
	  break;
	case BasicInstructionMathType::MUL:out << "*";
	  break;
	case BasicInstructionMathType::DIV:out << "/";
	  break;
	case BasicInstructionMathType::MOD:out << "%";
	  break;
	case BasicInstructionMathType::UNDEFINED:out << "UNDEFINED";
	  break;
  }
  out << " " << rightOperand;
}