#include "../../include/bblocks/BasicInstructionCondJumpVV.hpp"

BasicInstructionCondJumpVV::BasicInstructionCondJumpVV() {}

BasicInstructionCondJumpVV::BasicInstructionCondJumpVV(std::string leftVariableName, std::string rightVariableName,
													   BasicInstructionConditionType jumpCondition, std::string label,
													   std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware),
	  leftVariableName(leftVariableName),
	  rightVariableName(rightVariableName),
	  jumpCondition(jumpCondition),
	  label(label) {}

void BasicInstructionCondJumpVV::expandToHardwareInstructions() {
  machineCode.clear();

  switch (jumpCondition) {
	case BasicInstructionConditionType::LESS: {
	  auto conditionCalculation =
		  Utils::generateVVSubtraction(rightVariableName,
									   leftVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, label}});
	  break;
	}
	case BasicInstructionConditionType::LESS_EQUAL: {
	  auto conditionCalculation =
		  Utils::generateVVSubtraction(leftVariableName,
									   rightVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JZERO, label}});
	  break;
	}
	case BasicInstructionConditionType::EQUAL: {
	  const auto &endLabel = hardware->getLabel("jump", "end#", true);
	  auto conditionCalculation =
		  Utils::generateVVSubtraction(rightVariableName,
									   leftVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, endLabel}});
	  conditionCalculation =
		  Utils::generateVVSubtraction(leftVariableName,
									   rightVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, endLabel}});
	  machineCode.push_back(MachineCodeType{{{endLabel, 1}}, {HardwareInstruction::JUMP, label}});
	  break;
	}
	case BasicInstructionConditionType::NOT_EQUAL: {
	  auto conditionCalculation =
		  Utils::generateVVSubtraction(rightVariableName,
									   leftVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, label}});
	  conditionCalculation =
		  Utils::generateVVSubtraction(leftVariableName,
									   rightVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, label}});
	  break;
	}
  }
}

void BasicInstructionCondJumpVV::print(std::ostream &out) const {
  out << "JUMP IF " << leftVariableName << " ";
  switch (jumpCondition) {
	case BasicInstructionConditionType::LESS:out << "<";
	  break;
	case BasicInstructionConditionType::LESS_EQUAL:out << "<=";
	  break;
	case BasicInstructionConditionType::EQUAL:out << "==";
	  break;
	case BasicInstructionConditionType::NOT_EQUAL:out << "!=";
	  break;
  }

  out << " " << rightVariableName << " TO " << label;
}