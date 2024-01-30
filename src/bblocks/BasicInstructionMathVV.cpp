#include "../../include/bblocks/BasicInstructionMathVV.hpp"

BasicInstructionMathVV::BasicInstructionMathVV() {}

BasicInstructionMathVV::BasicInstructionMathVV(BasicInstructionMathType operationType, const std::string &leftOperand,
											   const std::string &rightOperand, const std::string &result,
											   std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware, BasicInstructionType::MATH_VV),
	  operationType(operationType),
	  leftOperand(leftOperand),
	  rightOperand(rightOperand),
	  result(result) {}

void BasicInstructionMathVV::expandToHardwareInstructions() {
  machineCode.clear();
  switch (operationType) {
	case BasicInstructionMathType::ADD: {
	  const auto &operations = Utils::generateVVAddition(leftOperand, rightOperand, result);
	  machineCode.insert(machineCode.end(), operations.begin(), operations.end());
	  break;
	}
	case BasicInstructionMathType::SUB: {
	  const auto &operations = Utils::generateVVSubtraction(leftOperand, rightOperand, result);
	  machineCode.insert(machineCode.end(), operations.begin(), operations.end());
	  break;
	}
	case BasicInstructionMathType::MUL: {
	  const auto &operations = Utils::generateVVMultiplication(leftOperand, rightOperand, result, hardware);
	  machineCode.insert(machineCode.end(), operations.begin(), operations.end());
	  break;
	}
	case BasicInstructionMathType::DIV: {
	  const auto &operations = Utils::generateVVDivision(leftOperand, rightOperand, result, hardware);
	  machineCode.insert(machineCode.end(), operations.begin(), operations.end());
	  break;
	}
	case BasicInstructionMathType::MOD: {
	  const auto &operations = Utils::generateVVModulo(leftOperand, rightOperand, result, hardware);
	  machineCode.insert(machineCode.end(), operations.begin(), operations.end());
	  break;
	}
	case BasicInstructionMathType::UNDEFINED: {
	  break;
	}
  }
}

void BasicInstructionMathVV::print(std::ostream &out) const {
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