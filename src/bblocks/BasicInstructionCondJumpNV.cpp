#include "../../include/bblocks/BasicInstructionCondJumpNV.hpp"

BasicInstructionCondJumpNV::BasicInstructionCondJumpNV() {}

BasicInstructionCondJumpNV::BasicInstructionCondJumpNV(std::uint64_t leftNumber, std::string rightVariableName,
													   BasicInstructionConditionType jumpCondition, std::string label,
													   std::shared_ptr<Hardware> hardware)
	: BasicInstruction(hardware, BasicInstructionType::COND_JUMP_NV),
	  leftNumber(leftNumber),
	  rightVariableName(rightVariableName),
	  jumpCondition(jumpCondition),
	  label(label) {}

void BasicInstructionCondJumpNV::expandToHardwareInstructions() {
  machineCode.clear();

  switch (jumpCondition) {
	case BasicInstructionConditionType::LESS: {
	  auto conditionCalculation = Utils::generateVNSubtraction(rightVariableName,
															   leftNumber,
															   Hardware::registerMap[Hardware::accumulator].name,
															   hardware);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, label}});
	  break;
	}
	case BasicInstructionConditionType::LESS_EQUAL: {
	  auto conditionCalculation =
		  Utils::generateNVSubtraction(leftNumber,
									   rightVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JZERO, label}});
	  break;
	}
	case BasicInstructionConditionType::EQUAL: {
	  const auto &endLabel = hardware->getLabel("jump", "end#", true);
	  auto conditionCalculation = Utils::generateVNSubtraction(rightVariableName,
															   leftNumber,
															   Hardware::registerMap[Hardware::accumulator].name,
															   hardware);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, endLabel}});
	  conditionCalculation =
		  Utils::generateNVSubtraction(leftNumber,
									   rightVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, endLabel}});
	  machineCode.push_back(MachineCodeType{{{endLabel, 1}}, {HardwareInstruction::JUMP, label}});
	  break;
	}
	case BasicInstructionConditionType::NOT_EQUAL: {
	  auto conditionCalculation = Utils::generateVNSubtraction(rightVariableName,
															   leftNumber,
															   Hardware::registerMap[Hardware::accumulator].name,
															   hardware);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, label}});
	  conditionCalculation =
		  Utils::generateNVSubtraction(leftNumber,
									   rightVariableName,
									   Hardware::registerMap[Hardware::accumulator].name);
	  machineCode.insert(machineCode.end(), conditionCalculation.begin(), conditionCalculation.end());
	  machineCode.push_back(MachineCodeType{{}, {HardwareInstruction::JPOS, label}});
	  break;
	}
  }
}

void BasicInstructionCondJumpNV::print(std::ostream &out) const {
  out << "JUMP IF " << leftNumber << " ";
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