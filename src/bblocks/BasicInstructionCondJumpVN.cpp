#include "../../include/bblocks/BasicInstructionCondJumpVN.hpp"

BasicInstructionCondJumpVN::BasicInstructionCondJumpVN() {}

BasicInstructionCondJumpVN::BasicInstructionCondJumpVN(std::string leftVariableName, std::uint64_t rightNumber,
													   BasicInstructionConditionType jumpCondition, std::string label,
													   std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware),
	  leftVariableName(leftVariableName),
	  rightNumber(rightNumber),
	  jumpCondition(jumpCondition),
	  label(label) {}

void BasicInstructionCondJumpVN::expandToHardwareInstructions() {
  machineCode.clear();

  switch (jumpCondition) {
	case BasicInstructionConditionType::LESS: {
	  auto conditionCalculation =
		  Utils::generateNVSubtraction(rightNumber,
									   leftVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, label}});
	  break;
	}
	case BasicInstructionConditionType::LESS_EQUAL: {
	  auto conditionCalculation = Utils::generateVNSubtraction(leftVariableName,
															   rightNumber,
															   Hardware::registerMap[Hardware::accumulator].name,
															   hardware);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JZERO, label}});
	  break;
	}
	case BasicInstructionConditionType::EQUAL: {
	  const auto &endLabel = hardware->getLabel("jump", "end#", true);
	  auto conditionCalculation =
		  Utils::generateNVSubtraction(rightNumber,
									   leftVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, endLabel}});
	  conditionCalculation = Utils::generateVNSubtraction(leftVariableName, rightNumber,
														  Hardware::registerMap[Hardware::accumulator].name, hardware);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, endLabel}});
	  machineCode.push_back(MachineCodeType{{{endLabel, 1}}, {HardwareInstruction::JUMP, label}});
	  break;
	}
	case BasicInstructionConditionType::NOT_EQUAL: {
	  auto conditionCalculation =
		  Utils::generateNVSubtraction(rightNumber,
									   leftVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, label}});
	  conditionCalculation = Utils::generateVNSubtraction(leftVariableName, rightNumber,
														  Hardware::registerMap[Hardware::accumulator].name, hardware);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, label}});
	  break;
	}
  }
}

void BasicInstructionCondJumpVN::print(std::ostream &out) const {
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

  out << " " << rightNumber << " TO " << label;
}