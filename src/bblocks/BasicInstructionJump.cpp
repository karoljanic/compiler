#include "../../include/bblocks/BasicInstructionJump.hpp"

BasicInstructionLabel::BasicInstructionLabel() {}

BasicInstructionLabel::BasicInstructionLabel(std::string label, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), label(label) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionLabel::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> result;
  result.push_back(std::make_pair(HardwareInstruction::LABEL, label));
  return result;
}

void BasicInstructionLabel::print(std::ostream& out) const {
  if (label.find("begin#") != std::string::npos) {
    out << std::endl;
  }
  out << "LABEL " << label;
  if (label.find("end#") != std::string::npos) {
    out << std::endl;
  }
}

BasicInstructionJump::BasicInstructionJump() {}

BasicInstructionJump::BasicInstructionJump(std::string label, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), label(label) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionJump::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> result;
  result.push_back(std::make_pair(HardwareInstruction::JUMP, label));
  return result;
}

void BasicInstructionJump::print(std::ostream& out) const {
  out << "JUMP TO " << label;
}

BasicInstructionConditionJumpVarVar::BasicInstructionConditionJumpVarVar() {}

BasicInstructionConditionJumpVarVar::BasicInstructionConditionJumpVarVar(std::string leftVariableName,
                                                                         std::string rightVariableName,
                                                                         BasicInstructionConditionType jumpCondition,
                                                                         std::string label, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware),
      leftVariableName(leftVariableName),
      rightVariableName(rightVariableName),
      jumpCondition(jumpCondition),
      label(label) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionConditionJumpVarVar::expandToHardwareInstructions()
    const {
  std::vector<std::pair<HardwareInstruction, std::string>> result;
  result.push_back(std::make_pair(HardwareInstruction::JUMP, label));
  return result;
}

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
                                                                         std::string label, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware),
      leftVariableName(leftVariableName),
      rightNumber(rightNumber),
      jumpCondition(jumpCondition),
      label(label) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionConditionJumpVarNum::expandToHardwareInstructions()
    const {
  std::vector<std::pair<HardwareInstruction, std::string>> result;
  result.push_back(std::make_pair(HardwareInstruction::JUMP, label));
  return result;
}

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
                                                                         std::string label, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware),
      leftNumber(leftNumber),
      rightVariableName(rightVariableName),
      jumpCondition(jumpCondition),
      label(label) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionConditionJumpNumVar::expandToHardwareInstructions()
    const {
  std::vector<std::pair<HardwareInstruction, std::string>> result;
  result.push_back(std::make_pair(HardwareInstruction::JUMP, label));
  return result;
}

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
                                                                         std::string label, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), leftNumber(leftNumber), rightNumber(rightNumber), jumpCondition(jumpCondition), label(label) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionConditionJumpNumNum::expandToHardwareInstructions()
    const {
  std::vector<std::pair<HardwareInstruction, std::string>> result;
  result.push_back(std::make_pair(HardwareInstruction::JUMP, label));
  return result;
}

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

BasicInstructionJumpRelative::BasicInstructionJumpRelative(std::string variableName, std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), registerName(variableName) {}

std::vector<std::pair<HardwareInstruction, std::string>> BasicInstructionJumpRelative::expandToHardwareInstructions() const {
  std::vector<std::pair<HardwareInstruction, std::string>> result;
  result.push_back(std::make_pair(HardwareInstruction::JUMPR, registerName));
  return result;
}

void BasicInstructionJumpRelative::print(std::ostream& out) const {
  out << "JUMP AT ADDRESS FROM " << registerName;
}
