#include "../../include/bblocks/BasicInstructionMathNN.hpp"

BasicInstructionMathNN::BasicInstructionMathNN() {}

BasicInstructionMathNN::BasicInstructionMathNN(BasicInstructionMathType operationType, uint64_t leftOperand,
                                               uint64_t rightOperand, const std::string& result,
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
      const auto& operations = Utils::generateNNAddition(leftOperand, rightOperand, result, hardware);
      machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
      break;
    }
    case BasicInstructionMathType::SUB: {
      const auto& operations = Utils::generateNNSubtraction(leftOperand, rightOperand, result);
      machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
      break;
    }
    case BasicInstructionMathType::MUL: {
      if (!Utils::multiplicationIsSafe(leftOperand, rightOperand)) {
        const auto& temp1 = hardware->getTempRegister();
        const auto& temp2 = hardware->getTempRegister();

        std::vector<std::pair<HardwareInstruction, std::string>> instructions;
        const auto& num1 = Utils::generateNumber(leftOperand, temp1);
        const auto& num2 = Utils::generateNumber(rightOperand, temp2);
        instructions.insert(instructions.begin(), num1.begin(), num1.end());
        instructions.insert(instructions.begin(), num2.begin(), num2.end());

        const auto& operations = Utils::generateVVMultiplication(temp1, temp2, result, hardware);
        machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
      }
      else {
        const auto& operations = Utils::generateNumber(leftOperand * rightOperand, result);
        machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
      }
    }
    case BasicInstructionMathType::DIV: {
      if (rightOperand == 0) {
        const auto& operations = Utils::generateNumber(0, result);
        machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
      }
      else {
        const auto& operations = Utils::generateNumber(leftOperand / rightOperand, result);
        machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
      }
    }
    case BasicInstructionMathType::MOD: {
      if (rightOperand == 0) {
        const auto& operations = Utils::generateNumber(0, result);
        machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
      }
      else {
        const auto& operations = Utils::generateNumber(leftOperand % rightOperand, result);
        machineCode.insert(machineCode.begin(), operations.begin(), operations.end());
      }
    }
    case BasicInstructionMathType::UNDEFINED: {
      break;
    }
  }
}

void BasicInstructionMathNN::print(std::ostream& out) const {
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