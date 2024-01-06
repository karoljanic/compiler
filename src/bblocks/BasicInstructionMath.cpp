#include "../../include/bblocks/BasicInstructionMath.hpp"

BasicInstructionMathOperationVarVar::BasicInstructionMathOperationVarVar() {}

BasicInstructionMathOperationVarVar::BasicInstructionMathOperationVarVar(BasicInstructionMathOperationType operationType,
                                                                         const std::string& leftOperand,
                                                                         const std::string& rightOperand,
                                                                         const std::string& result)
    : operationType(operationType), leftOperand(leftOperand), rightOperand(rightOperand), result(result) {}

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
                                                                         const std::string& result)
    : operationType(operationType), leftOperand(leftOperand), rightOperand(rightOperand), result(result) {}

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
                                                                         const std::string& result)
    : operationType(operationType), leftOperand(leftOperand), rightOperand(rightOperand), result(result) {}

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
                                                                         const std::string& result)
    : operationType(operationType), leftOperand(leftOperand), rightOperand(rightOperand), result(result) {}

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
