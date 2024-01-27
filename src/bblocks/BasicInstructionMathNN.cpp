#include "../../include/bblocks/BasicInstructionMathNN.hpp"

BasicInstructionMathNN::BasicInstructionMathNN() {}

BasicInstructionMathNN::BasicInstructionMathNN(BasicInstructionMathType operationType, uint64_t leftOperand,
											   uint64_t rightOperand, const std::string &result,
											   std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware),
	  operationType(operationType),
	  leftOperand(leftOperand),
	  rightOperand(rightOperand),
	  result(result) {}

void BasicInstructionMathNN::expandToHardwareInstructions() {
  machineCode.clear();
  switch (operationType) {
	case BasicInstructionMathType::ADD: {
	  const auto &operations = Utils::generateNNAddition(leftOperand, rightOperand, result, hardware);
	  machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
	  break;
	}
	case BasicInstructionMathType::SUB: {
	  const auto &operations = Utils::generateNNSubtraction(leftOperand, rightOperand, result);
	  machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
	  break;
	}
	case BasicInstructionMathType::MUL: {
	  const auto &operations = Utils::generateNNMultiplication(leftOperand, rightOperand, result, hardware);
	  machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
	  break;
	}
	case BasicInstructionMathType::DIV: {
	  const auto &operations = Utils::generateNNDivision(leftOperand, rightOperand, result);
	  machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
	  break;
	}
	case BasicInstructionMathType::MOD: {
	  const auto &operations = Utils::generateNNModulo(leftOperand, rightOperand, result);
	  machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
	  break;
	}
	case BasicInstructionMathType::UNDEFINED: {
	  break;
	}
  }
}

void BasicInstructionMathNN::print(std::ostream &out) const {
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