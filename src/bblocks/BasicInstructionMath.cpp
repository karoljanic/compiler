#include "../../include/bblocks/BasicInstructionMath.hpp"

BasicInstructionMathOperationVarVar::BasicInstructionMathOperationVarVar() {}

BasicInstructionMathOperationVarVar::BasicInstructionMathOperationVarVar(BasicInstructionMathOperationType operationType,
                                                                         const std::string& leftOperand,
                                                                         const std::string& rightOperand,
                                                                         const std::string& result,
                                                                         std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware),
      operationType(operationType),
      leftOperand(leftOperand),
      rightOperand(rightOperand),
      result(result) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMathOperationVarVar::expandToHardwareInstructions()
    const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  switch (operationType) {
    case BasicInstructionMathOperationType::ADD: {
      instructions.push_back({HardwareInstruction::GET, leftOperand});
      instructions.push_back({HardwareInstruction::ADD, rightOperand});
      instructions.push_back({HardwareInstruction::PUT, result});
      return instructions;
    }
    case BasicInstructionMathOperationType::SUB: {
      instructions.push_back({HardwareInstruction::GET, leftOperand});
      instructions.push_back({HardwareInstruction::SUB, rightOperand});
      instructions.push_back({HardwareInstruction::PUT, result});
      return instructions;
    }
    case BasicInstructionMathOperationType::MUL:
      return Utils::generateVarVarMultiplication(leftOperand, rightOperand, result, hardware);
    case BasicInstructionMathOperationType::DIV:
      return Utils::generateVarVarDivision(leftOperand, rightOperand);
    case BasicInstructionMathOperationType::MOD:
      return Utils::generateVarVarModulo(leftOperand, rightOperand);
    case BasicInstructionMathOperationType::UNDEFINED:
      return {};
  }
}

void BasicInstructionMathOperationVarVar::print(std::ostream& out) const {
  out << result << " = " << leftOperand << " ";
  switch (operationType) {
    case BasicInstructionMathOperationType::ADD:
      out << "+";
      break;
    case BasicInstructionMathOperationType::SUB:
      out << "-";
      break;
    case BasicInstructionMathOperationType::MUL:
      out << "*";
      break;
    case BasicInstructionMathOperationType::DIV:
      out << "/";
      break;
    case BasicInstructionMathOperationType::MOD:
      out << "%";
      break;
    case BasicInstructionMathOperationType::UNDEFINED:
      out << "UNDEFINED";
      break;
  }
  out << " " << rightOperand;
}

BasicInstructionMathOperationVarNum::BasicInstructionMathOperationVarNum() {}

BasicInstructionMathOperationVarNum::BasicInstructionMathOperationVarNum(BasicInstructionMathOperationType operationType,
                                                                         const std::string& leftOperand, uint64_t rightOperand,
                                                                         const std::string& result,
                                                                         std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware),
      operationType(operationType),
      leftOperand(leftOperand),
      rightOperand(rightOperand),
      result(result) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMathOperationVarNum::expandToHardwareInstructions()
    const {
  switch (operationType) {
    case BasicInstructionMathOperationType::ADD:
      return Utils::generateVarNumberAddition(leftOperand, rightOperand, result);
    case BasicInstructionMathOperationType::SUB:
      return Utils::generateVarNumberSubtraction(leftOperand, rightOperand, result, hardware);
    case BasicInstructionMathOperationType::MUL:
      return Utils::generateVarNumberMultiplication(leftOperand, rightOperand);
    case BasicInstructionMathOperationType::DIV:
      return Utils::generateVarNumberDivision(leftOperand, rightOperand);
    case BasicInstructionMathOperationType::MOD:
      return Utils::generateVarNumberModulo(leftOperand, rightOperand);
    case BasicInstructionMathOperationType::UNDEFINED:
      return {};
  }
}

void BasicInstructionMathOperationVarNum::print(std::ostream& out) const {
  out << result << " = " << leftOperand << " ";
  switch (operationType) {
    case BasicInstructionMathOperationType::ADD:
      out << "+";
      break;
    case BasicInstructionMathOperationType::SUB:
      out << "-";
      break;
    case BasicInstructionMathOperationType::MUL:
      out << "*";
      break;
    case BasicInstructionMathOperationType::DIV:
      out << "/";
      break;
    case BasicInstructionMathOperationType::MOD:
      out << "%";
      break;
    case BasicInstructionMathOperationType::UNDEFINED:
      out << "UNDEFINED";
      break;
  }
  out << " " << rightOperand;
}

BasicInstructionMathOperationNumVar::BasicInstructionMathOperationNumVar() {}

BasicInstructionMathOperationNumVar::BasicInstructionMathOperationNumVar(BasicInstructionMathOperationType operationType,
                                                                         uint64_t leftOperand, const std::string& rightOperand,
                                                                         const std::string& result,
                                                                         std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware),
      operationType(operationType),
      leftOperand(leftOperand),
      rightOperand(rightOperand),
      result(result) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMathOperationNumVar::expandToHardwareInstructions()
    const {
  std::vector<std::pair<HardwareInstruction, std::string>> instructions;
  switch (operationType) {
    case BasicInstructionMathOperationType::ADD:
      return Utils::generateVarNumberAddition(rightOperand, leftOperand, result);
    case BasicInstructionMathOperationType::SUB:
      return Utils::generateNumberVarSubtraction(leftOperand, rightOperand, result);
    case BasicInstructionMathOperationType::MUL:
      return Utils::generateVarNumberMultiplication(rightOperand, leftOperand);
    case BasicInstructionMathOperationType::DIV:
      return Utils::generateNumberVarDivision(rightOperand, leftOperand);
    case BasicInstructionMathOperationType::MOD:
      return Utils::generateNumberVarModulo(rightOperand, leftOperand);
    case BasicInstructionMathOperationType::UNDEFINED:
      return {};
  }
}

void BasicInstructionMathOperationNumVar::print(std::ostream& out) const {
  out << result << " = " << leftOperand << " ";
  switch (operationType) {
    case BasicInstructionMathOperationType::ADD:
      out << "+";
      break;
    case BasicInstructionMathOperationType::SUB:
      out << "-";
      break;
    case BasicInstructionMathOperationType::MUL:
      out << "*";
      break;
    case BasicInstructionMathOperationType::DIV:
      out << "/";
      break;
    case BasicInstructionMathOperationType::MOD:
      out << "%";
      break;
    case BasicInstructionMathOperationType::UNDEFINED:
      out << "UNDEFINED";
      break;
  }
  out << " " << rightOperand;
}

BasicInstructionMathOperationNumNum::BasicInstructionMathOperationNumNum() {}

BasicInstructionMathOperationNumNum::BasicInstructionMathOperationNumNum(BasicInstructionMathOperationType operationType,
                                                                         uint64_t leftOperand, uint64_t rightOperand,
                                                                         const std::string& result,
                                                                         std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware),
      operationType(operationType),
      leftOperand(leftOperand),
      rightOperand(rightOperand),
      result(result) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionMathOperationNumNum::expandToHardwareInstructions()
    const {
  switch (operationType) {
    case BasicInstructionMathOperationType::ADD: {
      if (!Utils::additionIsSafe(leftOperand, rightOperand)) {
        const auto& temp = hardware->getTempRegister();

        std::vector<std::pair<HardwareInstruction, std::string>> instructions;
        const auto& num1 = Utils::generateNumber(leftOperand, Hardware::registerMap[Hardware::accumulator].name);
        instructions.insert(instructions.begin(), num1.begin(), num1.end());
        const auto& num2 = Utils::generateNumber(rightOperand, temp);
        instructions.insert(instructions.begin(), num2.begin(), num2.end());
        instructions.push_back({HardwareInstruction::ADD, temp});
        instructions.push_back({HardwareInstruction::PUT, result});
        return instructions;
      }
      return Utils::generateNumber(leftOperand + rightOperand, result);
    }
    case BasicInstructionMathOperationType::SUB: {
      if (leftOperand < rightOperand) {
        return Utils::generateNumber(0, result);
      }
      else {
        return Utils::generateNumber(leftOperand - rightOperand, result);
      }
    }
    case BasicInstructionMathOperationType::MUL: {
      if (!Utils::multiplicationIsSafe(leftOperand, rightOperand)) {
        const auto& temp1 = hardware->getTempRegister();
        const auto& temp2 = hardware->getTempRegister();

        std::vector<std::pair<HardwareInstruction, std::string>> instructions;
        const auto& num1 = Utils::generateNumber(leftOperand, temp1);
        const auto& num2 = Utils::generateNumber(rightOperand, temp2);
        instructions.insert(instructions.begin(), num1.begin(), num1.end());
        instructions.insert(instructions.begin(), num2.begin(), num2.end());

        return Utils::generateVarVarMultiplication(temp1, temp2, result, hardware);
      }
      return Utils::generateNumber(leftOperand * rightOperand, result);
    }
    case BasicInstructionMathOperationType::DIV: {
      if (rightOperand == 0) {
        return Utils::generateNumber(0, result);
      }
      else {
        return Utils::generateNumber(leftOperand / rightOperand, result);
      }
    }
    case BasicInstructionMathOperationType::MOD: {
      if (rightOperand == 0) {
        return Utils::generateNumber(0, result);
      }
      else {
        return Utils::generateNumber(leftOperand % rightOperand, result);
      }
    }
    case BasicInstructionMathOperationType::UNDEFINED:
      return {};
  }
}

void BasicInstructionMathOperationNumNum::print(std::ostream& out) const {
  out << result << " = " << leftOperand << " ";
  switch (operationType) {
    case BasicInstructionMathOperationType::ADD:
      out << "+";
      break;
    case BasicInstructionMathOperationType::SUB:
      out << "-";
      break;
    case BasicInstructionMathOperationType::MUL:
      out << "*";
      break;
    case BasicInstructionMathOperationType::DIV:
      out << "/";
      break;
    case BasicInstructionMathOperationType::MOD:
      out << "%";
      break;
    case BasicInstructionMathOperationType::UNDEFINED:
      out << "UNDEFINED";
      break;
  }
  out << " " << rightOperand;
}
