#include "../../include/bblocks/BasicInstructionCondJumpNN.hpp"

BasicInstructionCondJumpNN::BasicInstructionCondJumpNN() {}

BasicInstructionCondJumpNN::BasicInstructionCondJumpNN(std::uint64_t leftNumber, std::uint64_t rightNumber,
                                                       BasicInstructionConditionType jumpCondition, std::string label,
                                                       std::shared_ptr<Hardware> hardware)
    : BasicInstruction(hardware), leftNumber(leftNumber), rightNumber(rightNumber), jumpCondition(jumpCondition), label(label) {}

void BasicInstructionCondJumpNN::expandToHardwareInstructions() {
  machineCode.clear();

  switch (jumpCondition) {
    case BasicInstructionConditionType::LESS: {
      auto conditionCalculation =
          Utils::generateNNSubtraction(rightNumber, leftNumber, Hardware::registerMap[Hardware::accumulator].name);
      machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
      machineCode.push_back(std::make_pair(HardwareInstruction::JPOS, label));
      break;
    }
    case BasicInstructionConditionType::LESS_EQUAL: {
      auto conditionCalculation =
          Utils::generateNNSubtraction(leftNumber, rightNumber, Hardware::registerMap[Hardware::accumulator].name);
      machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
      machineCode.push_back(std::make_pair(HardwareInstruction::JZERO, label));
      break;
    }
    case BasicInstructionConditionType::EQUAL: {
      const auto& endLabel = hardware->getLabel("jump", "end#", true);
      auto conditionCalculation =
          Utils::generateNNSubtraction(rightNumber, leftNumber, Hardware::registerMap[Hardware::accumulator].name);
      machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
      machineCode.push_back(std::make_pair(HardwareInstruction::JPOS, endLabel));
      conditionCalculation =
          Utils::generateNNSubtraction(leftNumber, rightNumber, Hardware::registerMap[Hardware::accumulator].name);
      machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
      machineCode.push_back(std::make_pair(HardwareInstruction::JPOS, endLabel));
      machineCode.push_back(std::make_pair(HardwareInstruction::JUMP, label));
      break;
    }
    case BasicInstructionConditionType::NOT_EQUAL: {
      auto conditionCalculation =
          Utils::generateNNSubtraction(rightNumber, leftNumber, Hardware::registerMap[Hardware::accumulator].name);
      machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
      machineCode.push_back(std::make_pair(HardwareInstruction::JPOS, label));
      conditionCalculation =
          Utils::generateNNSubtraction(leftNumber, rightNumber, Hardware::registerMap[Hardware::accumulator].name);
      machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
      machineCode.push_back(std::make_pair(HardwareInstruction::JPOS, label));
      break;
    }
  }
}

void BasicInstructionCondJumpNN::print(std::ostream& out) const {
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