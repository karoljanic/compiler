#include "../../include/bblocks/BasicInstructionMathVN.hpp"

BasicInstructionMathVN::BasicInstructionMathVN() {}

BasicInstructionMathVN::BasicInstructionMathVN(BasicInstructionMathType operationType, const std::string& leftOperand,
                                               uint64_t rightOperand, const std::string& result,
                                               std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware),
      operationType(operationType),
      leftOperand(leftOperand),
      rightOperand(rightOperand),
      result(result) {}

void BasicInstructionMathVN::expandToHardwareInstructions() {
  machineCode.clear();
  switch (operationType) {
    case BasicInstructionMathType::ADD: {
      const auto operations = Utils::generateVNAddition(leftOperand, rightOperand, result);
      machineCode.insert(machineCode.end(), operations.begin(), operations.end());
      break;
    }
    case BasicInstructionMathType::SUB: {
      const auto operations = Utils::generateVNSubtraction(leftOperand, rightOperand, result, hardware);
      machineCode.insert(machineCode.end(), operations.begin(), operations.end());
      break;
    }
    case BasicInstructionMathType::MUL: {
      const auto operations = Utils::generateVNMultiplication(leftOperand, rightOperand);
      machineCode.insert(machineCode.end(), operations.begin(), operations.end());
      break;
    }
    case BasicInstructionMathType::DIV: {
      const auto operations = Utils::generateNVDivision(leftOperand, rightOperand);
      machineCode.insert(machineCode.end(), operations.begin(), operations.end());
      break;
    }
    case BasicInstructionMathType::MOD: {
      const auto operations = Utils::generateNVModulo(leftOperand, rightOperand);
      machineCode.insert(machineCode.end(), operations.begin(), operations.end());
      break;
    }
    case BasicInstructionMathType::UNDEFINED: {
      break;
    }
  }
}

void BasicInstructionMathVN::print(std::ostream& out) const {
  out << result << " = " << leftOperand << " ";
  switch (operationType) {
    case BasicInstructionMathType::ADD:
      out << "+";
      break;
    case BasicInstructionMathType::SUB:
      out << "-";
      break;
    case BasicInstructionMathType::MUL:
      out << "*";
      break;
    case BasicInstructionMathType::DIV:
      out << "/";
      break;
    case BasicInstructionMathType::MOD:
      out << "%";
      break;
    case BasicInstructionMathType::UNDEFINED:
      out << "UNDEFINED";
      break;
  }
  out << " " << rightOperand;
}