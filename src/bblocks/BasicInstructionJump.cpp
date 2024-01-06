#include "../../include/bblocks/BasicInstructionJump.hpp"

BasicInstructionLabel::BasicInstructionLabel() {}

BasicInstructionLabel::BasicInstructionLabel(std::string label) : label(label) {}

void BasicInstructionLabel::print(std::ostream& out) const {
  out << "LABEL " << label;
}

BasicInstructionJump::BasicInstructionJump() {}

BasicInstructionJump::BasicInstructionJump(std::string label) : label(label) {}

void BasicInstructionJump::print(std::ostream& out) const {
  out << "JUMP TO " << label;
}

BasicInstructionConditionJumpVarVar::BasicInstructionConditionJumpVarVar() {}

BasicInstructionConditionJumpVarVar::BasicInstructionConditionJumpVarVar(std::string leftVariableName,
                                                                         std::string rightVariableName,
                                                                         BasicInstructionConditionType jumpCondition,
                                                                         std::string label)
    : leftVariableName(leftVariableName), rightVariableName(rightVariableName), jumpCondition(jumpCondition), label(label) {}

void BasicInstructionConditionJumpVarVar::print(std::ostream& out) const {
  out << "JUMP IF " << leftVariableName << " ";
  switch (jumpCondition) {
    case BasicInstructionConditionType::LESS:
      out << "<";
      break;
    case BasicInstructionConditionType::LESS_EQUAL:
      out << "<=";
      break;
    case BasicInstructionConditionType::EQUAL:
      out << "==";
      break;
    case BasicInstructionConditionType::NOT_EQUAL:
      out << "!=";
      break;
  }

  out << " " << rightVariableName << " TO " << label;
}

BasicInstructionConditionJumpVarNum::BasicInstructionConditionJumpVarNum() {}

BasicInstructionConditionJumpVarNum::BasicInstructionConditionJumpVarNum(std::string leftVariableName, std::uint64_t rightNumber,
                                                                         BasicInstructionConditionType jumpCondition,
                                                                         std::string label)
    : leftVariableName(leftVariableName), rightNumber(rightNumber), jumpCondition(jumpCondition), label(label) {}

void BasicInstructionConditionJumpVarNum::print(std::ostream& out) const {
  out << "JUMP IF " << leftVariableName << " ";
  switch (jumpCondition) {
    case BasicInstructionConditionType::LESS:
      out << "<";
      break;
    case BasicInstructionConditionType::LESS_EQUAL:
      out << "<=";
      break;
    case BasicInstructionConditionType::EQUAL:
      out << "==";
      break;
    case BasicInstructionConditionType::NOT_EQUAL:
      out << "!=";
      break;
  }

  out << " " << rightNumber << " TO " << label;
}

BasicInstructionConditionJumpNumVar::BasicInstructionConditionJumpNumVar() {}

BasicInstructionConditionJumpNumVar::BasicInstructionConditionJumpNumVar(std::uint64_t leftNumber, std::string rightVariableName,
                                                                         BasicInstructionConditionType jumpCondition,
                                                                         std::string label)
    : leftNumber(leftNumber), rightVariableName(rightVariableName), jumpCondition(jumpCondition), label(label) {}

void BasicInstructionConditionJumpNumVar::print(std::ostream& out) const {
  out << "JUMP IF " << leftNumber << " ";
  switch (jumpCondition) {
    case BasicInstructionConditionType::LESS:
      out << "<";
      break;
    case BasicInstructionConditionType::LESS_EQUAL:
      out << "<=";
      break;
    case BasicInstructionConditionType::EQUAL:
      out << "==";
      break;
    case BasicInstructionConditionType::NOT_EQUAL:
      out << "!=";
      break;
  }

  out << " " << rightVariableName << " TO " << label;
}

BasicInstructionConditionJumpNumNum::BasicInstructionConditionJumpNumNum() {}

BasicInstructionConditionJumpNumNum::BasicInstructionConditionJumpNumNum(std::uint64_t leftNumber, std::uint64_t rightNumber,
                                                                         BasicInstructionConditionType jumpCondition,
                                                                         std::string label)
    : leftNumber(leftNumber), rightNumber(rightNumber), jumpCondition(jumpCondition), label(label) {}

void BasicInstructionConditionJumpNumNum::print(std::ostream& out) const {
  out << "JUMP IF " << leftNumber << " ";
  switch (jumpCondition) {
    case BasicInstructionConditionType::LESS:
      out << "<";
      break;
    case BasicInstructionConditionType::LESS_EQUAL:
      out << "<=";
      break;
    case BasicInstructionConditionType::EQUAL:
      out << "==";
      break;
    case BasicInstructionConditionType::NOT_EQUAL:
      out << "!=";
      break;
  }

  out << " " << rightNumber << " TO " << label;
}

BasicInstructionJumpRelative::BasicInstructionJumpRelative() {}

BasicInstructionJumpRelative::BasicInstructionJumpRelative(std::string variableName) : registerName(variableName) {}

void BasicInstructionJumpRelative::print(std::ostream& out) const {
  out << "JUMP AT ADDRESS FROM " << registerName;
}
