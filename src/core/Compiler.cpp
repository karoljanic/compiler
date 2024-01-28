#include <iostream>

#include "../../include/core/Compiler.hpp"

Compiler::Compiler() {}

Compiler::Compiler(std::shared_ptr<AstNode> ast) : ast(ast), hardware(std::make_shared<Hardware>()) {}

void Compiler::generateMachineCode(std::ofstream &outputFile) {
  convertToFirstControlFlowGraph();
  pasteProcedures();
  convertToControlFlowGraph();
  optimizeControlFlowGraph();
  expandAndOptimizeBasicInstructions();
  findRegisters();
  optimizeMachineCode();
  findLabels();
  generateOutput(outputFile);
}

void Compiler::generateMachineCodeWithDebug(std::ofstream &astFile, std::ofstream &basicBlocksFile,
											std::ofstream &machineCodeFile, std::ofstream &outputFile) {
  ast->print(astFile, 0);

  convertToFirstControlFlowGraph();
  pasteProcedures();
  convertToControlFlowGraph();
  optimizeControlFlowGraph();
  expandAndOptimizeBasicInstructions();
  findRegisters();
  optimizeMachineCode();
  findLabels();
  generateOutput(outputFile);

  controlFlowGraph.printGraph(basicBlocksFile);
  for (const auto &instruction : machineCodeWithVariablesAndLabels) {
	for (const auto &label : instruction.getLabels()) {
	  machineCodeFile << "[[ " << label << " ]]" << std::endl;
	}
	for (const auto &machineCode : instruction.getInstruction()->getMachineCode()) {
	  machineCodeFile << Hardware::instructionMap[machineCode.second.first].opcode << " " << machineCode.second.second;
	  for (const auto &label : machineCode.first) {
		machineCodeFile << " [" << label.first << " " << label.second << "] ";
	  }
	  machineCodeFile << std::endl;
	}
  }
}

void Compiler::pasteProcedures() {
  const std::shared_ptr<AstProgram> &program = std::dynamic_pointer_cast<AstProgram>(ast);
  const std::shared_ptr<AstMain> &main = program->getMain();
  const std::shared_ptr<AstProcedures> &procedures = program->getProcedures();

  for (auto &procedure : procedures->getProcedures()) {
	std::shared_ptr<AstDeclarations> newProcedureDeclarations = std::make_shared<AstDeclarations>();
	std::shared_ptr<AstCommands> newProcedureCommands = std::make_shared<AstCommands>();
	pasteProceduresUtil(procedure->getDeclarations(), procedure->getCommands(), procedures,
						newProcedureDeclarations, newProcedureCommands);
	procedure->setDeclarations(newProcedureDeclarations);
	procedure->setCommands(newProcedureCommands);
  }

  std::shared_ptr<AstDeclarations> newMainDeclarations = std::make_shared<AstDeclarations>();
  std::shared_ptr<AstCommands> newMainCommands = std::make_shared<AstCommands>();
  pasteProceduresUtil(main->getDeclarations(), main->getCommands(), procedures, newMainDeclarations, newMainCommands);
  main->setDeclarations(newMainDeclarations);
  main->setCommands(newMainCommands);

  std::fstream astFile;
  astFile.open("ast2.txt", std::ios::out);
  ast->print(astFile, 0);
}

void Compiler::pasteProceduresUtil(const std::shared_ptr<AstDeclarations> &declarations,
								   const std::shared_ptr<AstCommands> &commands,
								   const std::shared_ptr<AstProcedures> &procedures,
								   std::shared_ptr<AstDeclarations> &newDeclarations,
								   std::shared_ptr<AstCommands> &newCommands) {

  std::map<std::string, std::string> emptyRewrite;
  for (const auto &declaration : declarations->getDeclarations()) {
	newDeclarations->addDeclaration(std::dynamic_pointer_cast<AstLeftValue>(declaration->copy(emptyRewrite)));
  }

  for (const auto &command : commands->getCommands()) {
	if (command->getCommandType() == AstCommandType::PROCEDURE_CALL) {
	  const auto &procedureCall = std::dynamic_pointer_cast<AstProcedureCall>(command);
	  const auto &procedureName = procedureCall->getName();
	  const auto &args = procedureCall->getArgsList()->getArgs();

	  std::set<std::string> argsSet;
	  for (const auto &arg : args) {
		argsSet.insert(arg->getName());
	  }
	  bool areArgsUnique = argsSet.size() == args.size();

	  if (!pasteProcedureEfficiency(procedureName) && areArgsUnique) {
		newCommands->addCommand(std::dynamic_pointer_cast<AstCommand>(command->copy(emptyRewrite)));
		usedProcedures.push_back(procedureName);
		continue;
	  }

	  // paste procedure
	  const auto &procedure = procedures->getProcedure(procedureName);
	  const auto &procedureHeader = procedure->getHeader();
	  const auto &procedureArgs = procedureHeader->getArgsDeclaration()->getArgs();
	  const auto &procedureDeclarations = procedure->getDeclarations()->getDeclarations();

	  std::map<std::string, std::string> namesRewrite;
	  for (size_t i = 0; i < procedureArgs.size(); i++) {
		namesRewrite[procedureArgs[i]->getName()] = args[i]->getName();
	  }

	  for (const auto &declaration : procedureDeclarations) {
		const auto decName = declaration->getName();
		const auto decNewName = procedureName + "#" + decName;
		namesRewrite[decName] = decNewName;
		newDeclarations->addDeclaration(std::dynamic_pointer_cast<AstLeftValue>(declaration->copy(namesRewrite)));
	  }

	  for (const auto &command : procedure->getCommands()->getCommands()) {
		newCommands->addCommand(std::dynamic_pointer_cast<AstCommand>(command->copy(namesRewrite)));
	  }
	} else {
	  newCommands->addCommand(std::dynamic_pointer_cast<AstCommand>(command->copy(emptyRewrite)));
	}
  }
}

void Compiler::convertToFirstControlFlowGraph() {
  const std::shared_ptr<AstProgram> program = std::dynamic_pointer_cast<AstProgram>(ast);
  const std::shared_ptr<AstProcedures> &procedures = program->getProcedures();

  for (const auto &procedure : procedures->getProcedures()) {
	currProcedureArgs.clear();
	std::pair<std::string, std::vector<ControlFlowGraphNode>> blockInstructions;
	parseAstProcedure(procedure, blockInstructions);
	currProcedureArgs.clear();
	firstControlFlowGraph.addSubgraph(blockInstructions.first, blockInstructions.second);
  }

  firstControlFlowGraph.expandInstructions();

  hardware = std::make_shared<Hardware>();
  scopes = std::stack<std::string>();
  translationTable = std::map<std::string, std::string>();
  parentsIds = std::vector<uint64_t>();
  currLabels = std::map < uint64_t, std::vector < std::string >> ();
  currProcedureArgs = std::vector<std::string>();
//  proceduresArgs = std::map < std::string, std::vector < std::string >> ();
}

void Compiler::convertToControlFlowGraph() {
  const std::shared_ptr<AstProgram> program = std::dynamic_pointer_cast<AstProgram>(ast);
  const std::shared_ptr<AstMain> &main = program->getMain();
  const std::shared_ptr<AstProcedures> &procedures = program->getProcedures();

  for (const auto &procedure : procedures->getProcedures()) {
	currProcedureArgs.clear();
	std::pair<std::string, std::vector<ControlFlowGraphNode>> blockInstructions;
	parseAstProcedure(procedure, blockInstructions);
	currProcedureArgs.clear();
	controlFlowGraph.addSubgraph(blockInstructions.first, blockInstructions.second);
  }

  std::pair<std::string, std::vector<ControlFlowGraphNode>> blockInstructions;
  parseAstMain(main, blockInstructions);
  controlFlowGraph.addSubgraph(blockInstructions.first, blockInstructions.second);

  controlFlowGraph.initializeSuccessorsBasedOnPredecessors();

  for (auto &node : controlFlowGraph.getNodes("main")) {
	if (currLabels.find(node.getId()) != currLabels.end()) {
	  for (const auto &label : currLabels[node.getId()]) {
		node.addLabel(label);
	  }
	}
  }

  const auto subgraphNames = controlFlowGraph.getSubgraphNames();
  for (const auto &subgraphName : subgraphNames) {
	if (subgraphName == "main") {
	  continue;
	}
	for (auto &node : controlFlowGraph.getNodes(subgraphName)) {
	  if (currLabels.find(node.getId()) != currLabels.end()) {
		for (const auto &label : currLabels[node.getId()]) {
		  node.addLabel(label);
		}
	  }
	}
  }
}

void Compiler::optimizeControlFlowGraph() {

}

void Compiler::expandAndOptimizeBasicInstructions() {
  controlFlowGraph.expandInstructions();

  controlFlowGraph.optimize();

  const auto &mainCode = controlFlowGraph.getNodes("main");
  machineCodeWithVariablesAndLabels.insert(machineCodeWithVariablesAndLabels.end(), mainCode.begin(), mainCode.end());

  const auto subgraphNames = controlFlowGraph.getSubgraphNames();
  for (const auto &subgraphName : subgraphNames) {
	if (subgraphName == "main") {
	  continue;
	}
	if (std::find(usedProcedures.begin(), usedProcedures.end(), subgraphName) == usedProcedures.end()) {
	  continue;
	}
	const auto functionCode = controlFlowGraph.getNodes(subgraphName);
	machineCodeWithVariablesAndLabels.insert(machineCodeWithVariablesAndLabels.end(), functionCode.begin(),
											 functionCode.end());
  }
}

void Compiler::findRegisters() {
  const auto liveRanges = controlFlowGraph.calculateVariablesLiveRanges();
//  for (const auto &liveRange : liveRanges) {
//	std::cout << liveRange.first << ": " << std::endl;
//	for (const auto &range : liveRange.second) {
//	  for (const auto &el : range) {
//		std::cout << el << " ";
//	  }
//	  std::cout << std::endl;
//	}
//	std::cout << std::endl;
//  }

  const auto &definedVariables = controlFlowGraph.getDefinedVariables();
  const auto &usedVariables = controlFlowGraph.getUsedVariables();

  registersLinearScan = RegistersLinearScan{hardware};
  for (const auto &var : definedVariables) {
	registersLinearScan.addVariableUsageInfo(var.second, var.first, UsageType::LVAL);
  }

  for (const auto &var : usedVariables) {
	registersLinearScan.addVariableUsageInfo(var.second, var.first, UsageType::RVAL);
  }

  registersLinearScan.createRanges(controlFlowGraph);
  registersLinearScan.allocateRegisters();

  for (const auto &node : machineCodeWithVariablesAndLabels) {
	const auto memoryLoads = registersLinearScan.getMemoryLoad(node.getId());
	const auto memoryStores = registersLinearScan.getMemoryStore(node.getId());

	if (node.getInstruction()->getMachineCode().size() == 1
		&& node.getInstruction()->getMachineCode()[0].second.first == HardwareInstruction::SAVE_STATE) {
	  continue;
	} else if (node.getInstruction()->getMachineCode().size() == 1
		&& node.getInstruction()->getMachineCode()[0].second.first == HardwareInstruction::RESTORE_STATE) {
	  continue;
	}

	bool isFirstInstruction = true;
	std::vector<std::pair<std::string, uint64_t>> firstInstructionLabels;
	for (const auto &label : node.getLabels()) {
	  firstInstructionLabels.push_back(std::make_pair(label, 0));
	}

	for (const auto &memoryLoad : memoryLoads) {
	  //std::cout << "LOAD: " << memoryLoad.first << " " << memoryLoad.second << " " << node.getId() << std::endl;

	  const auto accumulator = Hardware::registerMap[Hardware::accumulator].name;
	  const auto generateNumber = Utils::generateNumber(memoryLoad.second, accumulator);
	  for (const auto &instruction : generateNumber) {
		auto instructionCopy = instruction;
		if (isFirstInstruction) {
		  instructionCopy.first.insert(instructionCopy.first.end(), firstInstructionLabels.begin(),
									   firstInstructionLabels.end());
		  isFirstInstruction = false;
		}

		machineCodeWithLabels.push_back(instructionCopy);
	  }

	  machineCodeWithLabels.push_back(MachineCodeType{{}, std::make_pair(HardwareInstruction::LOAD, accumulator)});
	  machineCodeWithLabels.push_back(MachineCodeType{{}, std::make_pair(HardwareInstruction::PUT, memoryLoad.first)});
	}

	for (const auto &instruction : node.getInstruction()->getMachineCode()) {
	  auto instructionCopy = instruction;
	  if (isFirstInstruction) {
		instructionCopy.first.insert(instructionCopy.first.end(), firstInstructionLabels.begin(),
									 firstInstructionLabels.end());
		isFirstInstruction = false;
	  }

	  instructionCopy.second.second =
		  registersLinearScan.getVariableRegister(instruction.second.second, node.getId());
	  machineCodeWithLabels.push_back(instructionCopy);
	}

	for (const auto &memoryStore : memoryStores) {
	  //std::cout << "STORE: " << memoryStore.first << " " << memoryStore.second << " " << node.getId() << std::endl;

	  const auto generateNumber = Utils::generateNumber(memoryStore.second, memoryStore.first);
	  machineCodeWithLabels.push_back(MachineCodeType{{}, std::make_pair(HardwareInstruction::GET, memoryStore.first)});
	  machineCodeWithLabels.insert(machineCodeWithLabels.end(), generateNumber.begin(), generateNumber.end());
	  machineCodeWithLabels.push_back(MachineCodeType{{},
													  std::make_pair(HardwareInstruction::STORE, memoryStore.first)});
	  machineCodeWithLabels.push_back(MachineCodeType{{}, std::make_pair(HardwareInstruction::PUT, memoryStore.first)});
	}
  }
}

void Compiler::optimizeMachineCode() {}

void Compiler::findLabels() {
  std::map<std::string, uint64_t> labels;
  uint64_t lineCounter{0};
  for (const auto &instruction : machineCodeWithLabels) {
	for (const auto &label : instruction.first) {
	  if (label.first != "") {
		labels[label.first] = lineCounter + label.second;
	  }
	}

	machineCode.emplace_back(instruction.second);
	lineCounter++;
  }

  for (const auto &label : labels) {
	std::cout << label.first << " " << label.second << std::endl;
  }

  for (auto &instruction : machineCode) {
	if (labels.find(instruction.second) != labels.end()) {
	  instruction.second = std::to_string(labels[instruction.second]);
	}
  }
}

void Compiler::generateOutput(std::ofstream &outputFile) {
  for (const auto &instruction : machineCode) {
	outputFile << Hardware::instructionMap[instruction.first].opcode << " " << instruction.second << std::endl;
  }
}

void Compiler::parseAstMain(std::shared_ptr<AstMain> node,
							std::pair<std::string, std::vector<ControlFlowGraphNode>> &instructions) {

  instructions.first = "main";
  instructions.second.clear();

  scopes.push("main");

  parseAstDeclarations(node->getDeclarations());

  parentsIds.clear();

  const auto &commands = node->getCommands()->getCommands();
  auto commandsInstructions = std::vector<ControlFlowGraphNode>();
  for (size_t index = 0; index < commands.size(); index++) {
	parseAstCommand(commands[index], commandsInstructions);
  }

  ControlFlowGraphNode haltNode;
  haltNode.setInstruction(std::make_shared<BasicInstructionHalt>(hardware));
  haltNode.addPredecessors(parentsIds);

  instructions.second.insert(instructions.second.end(), commandsInstructions.begin(), commandsInstructions.end());
  instructions.second.emplace_back(haltNode);

  scopes.pop();
}

void Compiler::parseAstProcedure(std::shared_ptr<AstProcedure> node,
								 std::pair<std::string, std::vector<ControlFlowGraphNode>> &instructions) {
  const auto &procedureName = node->getHeader()->getName();
  const auto &args = node->getHeader()->getArgsDeclaration()->getArgs();

  instructions.first = procedureName;
  instructions.second.clear();

  scopes.push(procedureName);

  parseAstProcedureHeader(node->getHeader());
  parseAstDeclarations(node->getDeclarations());

  parentsIds.clear();

  std::vector<std::string> argNames;
  for (const auto &arg : args) {
	argNames.push_back(translationTable[arg->getName()]);
  }

  const auto functionRecordAddress = hardware->getFunctionRecordAddress(procedureName);
  const auto &functionBeginLabel = hardware->getGlobalLabel(procedureName, "begin#");
  const auto returnAddress = hardware->getTempRegister();

  ControlFlowGraphNode popNode;
  popNode.setInstruction(std::make_shared<BasicInstructionPop>(functionRecordAddress,
															   argNames,
															   returnAddress,
															   hardware));
  popNode.addPredecessors(parentsIds);
  popNode.addLabel(functionBeginLabel);
  instructions.second.push_back(popNode);
  parentsIds = {popNode.getId()};

  const auto &commands = node->getCommands()->getCommands();
  auto commandsInstructions = std::vector<ControlFlowGraphNode>();
  for (size_t index = 0; index < commands.size(); index++) {
	parseAstCommand(commands[index], commandsInstructions);
  }
  instructions.second.insert(instructions.second.end(), commandsInstructions.begin(), commandsInstructions.end());

  ControlFlowGraphNode pushNode;
  pushNode.setInstruction(std::make_shared<BasicInstructionPush>(functionRecordAddress,
																 argNames,
																 hardware));
  pushNode.addPredecessors(parentsIds);
  instructions.second.push_back(pushNode);
  parentsIds = {pushNode.getId()};

  ControlFlowGraphNode returnNode;
  returnNode.setInstruction(std::make_shared<BasicInstructionJumpRelative>(returnAddress, hardware));
  returnNode.addPredecessors(parentsIds);
  instructions.second.emplace_back(returnNode);
  parentsIds = {};

  scopes.pop();
}

void Compiler::parseAstCommand(std::shared_ptr<AstCommand> node, std::vector<ControlFlowGraphNode> &instructions) {
  switch (node->getCommandType()) {
	case AstCommandType::READ: {
	  const auto &read = std::dynamic_pointer_cast<AstRead>(node);
	  const auto &leftValue = read->getLvalue();
	  if (leftValue->getNodeType() == AstNodeType::VARIABLE) {
		const auto &var = std::dynamic_pointer_cast<AstVariable>(leftValue);

		ControlFlowGraphNode readNode;
		readNode.setInstruction(std::make_shared<BasicInstructionRead>(translationTable[var->getName()], hardware));
		readNode.addPredecessors(parentsIds);
		instructions.push_back(readNode);
		parentsIds = {readNode.getId()};
	  } else {
		const auto &arr = std::dynamic_pointer_cast<AstArray>(leftValue);
		const auto &arrIndex = arr->getArgument();
		if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
		  const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
		  const auto &temp = hardware->getTempRegister();

		  ControlFlowGraphNode readNode;
		  readNode.setInstruction(std::make_shared<BasicInstructionRead>(temp, hardware));
		  readNode.addPredecessors(parentsIds);
		  instructions.push_back(readNode);
		  parentsIds = {readNode.getId()};

		  ControlFlowGraphNode moveNode;
		  moveNode.setInstruction(std::make_shared<BasicInstructionMovVKA>(temp,
																		   translationTable[arr->getName()],
																		   arrIndexNum->getValue(),
																		   isProcedureArgument(translationTable[arr->getName()]),
																		   hardware));
		  moveNode.addPredecessors(parentsIds);
		  instructions.push_back(moveNode);
		  parentsIds = {moveNode.getId()};
		} else {
		  const auto &arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
		  const auto &temp = hardware->getTempRegister();

		  ControlFlowGraphNode readNode;
		  readNode.setInstruction(std::make_shared<BasicInstructionRead>(temp, hardware));
		  readNode.addPredecessors(parentsIds);
		  instructions.push_back(readNode);
		  parentsIds = {readNode.getId()};

		  ControlFlowGraphNode moveNode;
		  moveNode.setInstruction(std::make_shared<BasicInstructionMovVA>(temp,
																		  translationTable[arr->getName()],
																		  translationTable[arrIndexVar->getName()],
																		  isProcedureArgument(translationTable[arr->getName()]),
																		  hardware));
		  moveNode.addPredecessors(parentsIds);
		  instructions.push_back(moveNode);
		  parentsIds = {moveNode.getId()};
		}
	  }
	  break;
	}
	case AstCommandType::WRITE: {
	  const auto &write = std::dynamic_pointer_cast<AstWrite>(node);
	  const auto &value = write->getValue();
	  if (value->getNodeType() == AstNodeType::NUMBER) {
		const auto &num = std::dynamic_pointer_cast<AstNumber>(value);
		const auto &acc = hardware->getTempRegister();

		ControlFlowGraphNode moveNode;
		moveNode.setInstruction(std::make_shared<BasicInstructionMovNV>(num->getValue(), acc, hardware));
		moveNode.addPredecessors(parentsIds);
		instructions.push_back(moveNode);
		parentsIds = {moveNode.getId()};

		ControlFlowGraphNode writeNode;
		writeNode.setInstruction(std::make_shared<BasicInstructionWrite>(acc, hardware));
		writeNode.addPredecessors(parentsIds);
		instructions.push_back(writeNode);
		parentsIds = {writeNode.getId()};
	  } else if (value->getNodeType() == AstNodeType::VARIABLE) {
		const auto &var = std::dynamic_pointer_cast<AstVariable>(value);

		ControlFlowGraphNode writeNode;
		writeNode.setInstruction(std::make_shared<BasicInstructionWrite>(translationTable[var->getName()], hardware));
		writeNode.addPredecessors(parentsIds);
		instructions.push_back(writeNode);
		parentsIds = {writeNode.getId()};
	  } else {
		const auto &arr = std::dynamic_pointer_cast<AstArray>(value);
		const auto &arrIndex = arr->getArgument();
		if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
		  const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
		  const auto &temp = hardware->getTempRegister();

		  ControlFlowGraphNode moveNode;
		  moveNode.setInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[arr->getName()],
																		   arrIndexNum->getValue(),
																		   temp,
																		   isProcedureArgument(translationTable[arr->getName()]),
																		   hardware));
		  moveNode.addPredecessors(parentsIds);
		  instructions.push_back(moveNode);
		  parentsIds = {moveNode.getId()};

		  ControlFlowGraphNode writeNode;
		  writeNode.setInstruction(std::make_shared<BasicInstructionWrite>(temp, hardware));
		  writeNode.addPredecessors(parentsIds);
		  instructions.push_back(writeNode);
		  parentsIds = {writeNode.getId()};
		} else {
		  const auto &arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
		  const auto &temp = hardware->getTempRegister();

		  ControlFlowGraphNode moveNode;
		  moveNode.setInstruction(std::make_shared<BasicInstructionMovAV>(translationTable[arr->getName()],
																		  translationTable[arrIndexVar->getName()],
																		  temp,
																		  isProcedureArgument(translationTable[arr->getName()]),
																		  hardware));
		  moveNode.addPredecessors(parentsIds);
		  instructions.push_back(moveNode);
		  parentsIds = {moveNode.getId()};

		  ControlFlowGraphNode writeNode;
		  writeNode.setInstruction(std::make_shared<BasicInstructionWrite>(temp, hardware));
		  writeNode.addPredecessors(parentsIds);
		  instructions.push_back(writeNode);
		  parentsIds = {writeNode.getId()};
		}
	  }
	  break;
	}
	case AstCommandType::ASSIGNMENT: {
	  const auto &assignment = std::dynamic_pointer_cast<AstAssignment>(node);
	  const auto &leftValue = assignment->getLvalue();
	  const auto &expression = assignment->getExpression();
	  const auto
		  &tempResult = leftValue->getNodeType() == AstNodeType::VARIABLE ? translationTable[leftValue->getName()]
																		  : hardware->getTempRegister();

	  if (expression->getRight() == nullptr) {
		ControlFlowGraphNode tmpMoveNode;
		const auto &newValue = expression->getLeft();
		if (newValue->getNodeType() == AstNodeType::NUMBER) {
		  const auto &newNum = std::dynamic_pointer_cast<AstNumber>(newValue);

		  tmpMoveNode.setInstruction(std::make_shared<BasicInstructionMovNV>(newNum->getValue(), tempResult, hardware));
		} else if (newValue->getNodeType() == AstNodeType::VARIABLE) {
		  const auto &newVar = std::dynamic_pointer_cast<AstVariable>(newValue);

		  tmpMoveNode.setInstruction(std::make_shared<BasicInstructionMovVV>(translationTable[newVar->getName()],
																			 tempResult,
																			 hardware));
		} else {
		  const auto &newArr = std::dynamic_pointer_cast<AstArray>(newValue);
		  const auto &newArrIndex = newArr->getArgument();
		  if (newArrIndex->getNodeType() == AstNodeType::NUMBER) {

			tmpMoveNode.setInstruction(std::make_shared<BasicInstructionMovKAV>(
				translationTable[newArr->getName()],
				std::dynamic_pointer_cast<AstNumber>(newArrIndex)->getValue(),
				tempResult, isProcedureArgument(translationTable[newArr->getName()]),
				hardware));
		  } else {
			tmpMoveNode.setInstruction(std::make_shared<BasicInstructionMovAV>(
				translationTable[newArr->getName()],
				translationTable[std::dynamic_pointer_cast<AstVariable>(newArrIndex)->getName()],
				tempResult, isProcedureArgument(translationTable[newArr->getName()]),
				hardware));
		  }
		}

		ControlFlowGraphNode resultMoveNode;
		if (leftValue->getNodeType() == AstNodeType::VARIABLE) {
		  const auto &var = std::dynamic_pointer_cast<AstVariable>(leftValue);

		  resultMoveNode.setInstruction(std::make_shared<BasicInstructionMovVV>(tempResult,
																				translationTable[var->getName()],
																				hardware));
		} else {
		  const auto &arr = std::dynamic_pointer_cast<AstArray>(leftValue);
		  const auto &arrIndex = arr->getArgument();
		  if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
			const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);

			resultMoveNode.setInstruction(std::make_shared<BasicInstructionMovVKA>(tempResult,
																				   translationTable[arr->getName()],
																				   arrIndexNum->getValue(),
																				   isProcedureArgument(translationTable[arr->getName()]),
																				   hardware));
		  } else {
			const auto &arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);

			resultMoveNode.setInstruction(std::make_shared<BasicInstructionMovVA>(tempResult,
																				  translationTable[arr->getName()],
																				  translationTable[arrIndexVar->getName()],
																				  isProcedureArgument(translationTable[arr->getName()]),
																				  hardware));
		  }
		}

		tmpMoveNode.addPredecessors(parentsIds);
		instructions.push_back(tmpMoveNode);
		parentsIds = {tmpMoveNode.getId()};

		resultMoveNode.addPredecessors(parentsIds);
		instructions.push_back(resultMoveNode);
		parentsIds = {resultMoveNode.getId()};
	  } else {
		auto expressionType = BasicInstructionMathType::UNDEFINED;
		switch (expression->getExpressionType()) {
		  case AstExpressionType::ADD: {
			expressionType = BasicInstructionMathType::ADD;
			break;
		  }
		  case AstExpressionType::SUB: {
			expressionType = BasicInstructionMathType::SUB;
			break;
		  }
		  case AstExpressionType::MUL: {
			expressionType = BasicInstructionMathType::MUL;
			break;
		  }
		  case AstExpressionType::DIV: {
			expressionType = BasicInstructionMathType::DIV;
			break;
		  }
		  case AstExpressionType::MOD: {
			expressionType = BasicInstructionMathType::MOD;
			break;
		  }
		  default: {
			std::cout << "Undefined expression type" << std::endl;
			break;
		  }
		}
		const auto &leftOperand = expression->getLeft();
		const auto &rightOperand = expression->getRight();

		if (leftOperand->getNodeType() == AstNodeType::NUMBER && rightOperand->getNodeType() == AstNodeType::NUMBER) {
		  const auto &leftNum = std::dynamic_pointer_cast<AstNumber>(leftOperand);
		  const auto &rightNum = std::dynamic_pointer_cast<AstNumber>(rightOperand);

		  ControlFlowGraphNode mathNode;
		  mathNode.setInstruction(std::make_shared<BasicInstructionMathNN>(expressionType,
																		   leftNum->getValue(),
																		   rightNum->getValue(),
																		   tempResult,
																		   hardware));
		  mathNode.addPredecessors(parentsIds);
		  instructions.push_back(mathNode);
		  parentsIds = {mathNode.getId()};
		} else if (leftOperand->getNodeType() == AstNodeType::NUMBER) {
		  const auto &leftNum = std::dynamic_pointer_cast<AstNumber>(leftOperand);

		  if (rightOperand->getNodeType() == AstNodeType::VARIABLE) {
			const auto &rightVar = std::dynamic_pointer_cast<AstVariable>(rightOperand);
			ControlFlowGraphNode mathNode;
			mathNode.setInstruction(std::make_shared<BasicInstructionMathNV>(
				expressionType, leftNum->getValue(), translationTable[rightVar->getName()], tempResult, hardware));

			mathNode.addPredecessors(parentsIds);
			instructions.push_back(mathNode);
			parentsIds = {mathNode.getId()};
		  } else {
			const auto &rightArr = std::dynamic_pointer_cast<AstArray>(rightOperand);
			const auto &rightArrIndex = rightArr->getArgument();
			const auto &rightTemp = hardware->getTempRegister();
			ControlFlowGraphNode rightMoveNode;
			if (rightArrIndex->getNodeType() == AstNodeType::NUMBER) {
			  const auto rightArrIndexNum = std::dynamic_pointer_cast<AstNumber>(rightArrIndex);

			  rightMoveNode.setInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[rightArr->getName()],
																					rightArrIndexNum->getValue(),
																					rightTemp,
																					isProcedureArgument(translationTable[rightArr->getName()]),
																					hardware));
			} else {
			  const auto &rightArrIndexVar = std::dynamic_pointer_cast<AstVariable>(rightArrIndex);

			  rightMoveNode.setInstruction(std::make_shared<BasicInstructionMovAV>(
				  translationTable[rightArr->getName()],
				  translationTable[rightArrIndexVar->getName()],
				  rightTemp, isProcedureArgument(translationTable[rightArr->getName()]),
				  hardware));
			}

			rightMoveNode.addPredecessors(parentsIds);
			instructions.push_back(rightMoveNode);
			parentsIds = {rightMoveNode.getId()};
			ControlFlowGraphNode mathNode;
			mathNode.setInstruction(std::make_shared<BasicInstructionMathNV>(expressionType,
																			 leftNum->getValue(),
																			 rightTemp,
																			 tempResult,
																			 hardware));

			mathNode.addPredecessors(parentsIds);
			instructions.push_back(mathNode);
			parentsIds = {mathNode.getId()};
		  }
		} else if (rightOperand->getNodeType() == AstNodeType::NUMBER) {
		  const auto &rightNum = std::dynamic_pointer_cast<AstNumber>(rightOperand);

		  if (leftOperand->getNodeType() == AstNodeType::VARIABLE) {
			const auto &leftVar = std::dynamic_pointer_cast<AstVariable>(leftOperand);

			ControlFlowGraphNode mathNode;
			mathNode.setInstruction(std::make_shared<BasicInstructionMathVN>(
				expressionType, translationTable[leftVar->getName()], rightNum->getValue(), tempResult, hardware));

			mathNode.addPredecessors(parentsIds);
			instructions.push_back(mathNode);
			parentsIds = {mathNode.getId()};
		  } else {
			const auto &leftArr = std::dynamic_pointer_cast<AstArray>(leftOperand);
			const auto &leftArrIndex = leftArr->getArgument();
			const auto &leftTemp = hardware->getTempRegister();
			ControlFlowGraphNode leftMoveNode;
			if (leftArrIndex->getNodeType() == AstNodeType::NUMBER) {
			  const auto leftArrIndexNum = std::dynamic_pointer_cast<AstNumber>(leftArrIndex);

			  leftMoveNode.setInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[leftArr->getName()],
																				   leftArrIndexNum->getValue(),
																				   leftTemp,
																				   isProcedureArgument(translationTable[leftArr->getName()]),
																				   hardware));
			} else {
			  const auto &leftArrIndexVar = std::dynamic_pointer_cast<AstVariable>(leftArrIndex);

			  leftMoveNode.setInstruction(std::make_shared<BasicInstructionMovAV>(
				  translationTable[leftArr->getName()],
				  translationTable[leftArrIndexVar->getName()],
				  leftTemp, isProcedureArgument(translationTable[leftArr->getName()]),
				  hardware));
			}

			leftMoveNode.addPredecessors(parentsIds);
			instructions.push_back(leftMoveNode);
			parentsIds = {leftMoveNode.getId()};
			ControlFlowGraphNode mathNode;
			mathNode.setInstruction(std::make_shared<BasicInstructionMathVN>(expressionType,
																			 leftTemp,
																			 rightNum->getValue(),
																			 tempResult,
																			 hardware));
			mathNode.addPredecessors(parentsIds);
			instructions.push_back(mathNode);
			parentsIds = {mathNode.getId()};
		  }
		} else {
		  const auto &leftTemp = hardware->getTempRegister();
		  ControlFlowGraphNode leftMoveNode;
		  if (leftOperand->getNodeType() == AstNodeType::VARIABLE) {
			const auto &leftVar = std::dynamic_pointer_cast<AstVariable>(leftOperand);

			leftMoveNode.setInstruction(std::make_shared<BasicInstructionMovVV>(translationTable[leftVar->getName()],
																				leftTemp,
																				hardware));
		  } else {
			const auto &leftArr = std::dynamic_pointer_cast<AstArray>(leftOperand);
			const auto &leftArrIndex = leftArr->getArgument();
			if (leftArrIndex->getNodeType() == AstNodeType::NUMBER) {
			  const auto leftArrIndexNum = std::dynamic_pointer_cast<AstNumber>(leftArrIndex);

			  leftMoveNode.setInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[leftArr->getName()],
																				   leftArrIndexNum->getValue(),
																				   leftTemp,
																				   isProcedureArgument(translationTable[leftArr->getName()]),
																				   hardware));
			} else {
			  const auto &leftArrIndexVar = std::dynamic_pointer_cast<AstVariable>(leftArrIndex);

			  leftMoveNode.setInstruction(std::make_shared<BasicInstructionMovAV>(
				  translationTable[leftArr->getName()],
				  translationTable[leftArrIndexVar->getName()],
				  leftTemp, isProcedureArgument(translationTable[leftArr->getName()]),
				  hardware));
			}
		  }

		  leftMoveNode.addPredecessors(parentsIds);
		  instructions.push_back(leftMoveNode);
		  parentsIds = {leftMoveNode.getId()};

		  const auto &rightTemp = hardware->getTempRegister();
		  ControlFlowGraphNode rightMoveNode;
		  if (rightOperand->getNodeType() == AstNodeType::VARIABLE) {
			const auto &rightVar = std::dynamic_pointer_cast<AstVariable>(rightOperand);

			rightMoveNode.setInstruction(std::make_shared<BasicInstructionMovVV>(translationTable[rightVar->getName()],
																				 rightTemp,
																				 hardware));
		  } else {
			const auto &rightArr = std::dynamic_pointer_cast<AstArray>(rightOperand);
			const auto &rightArrIndex = rightArr->getArgument();
			if (rightArrIndex->getNodeType() == AstNodeType::NUMBER) {
			  const auto rightArrIndexNum = std::dynamic_pointer_cast<AstNumber>(rightArrIndex);

			  rightMoveNode.setInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[rightArr->getName()],
																					rightArrIndexNum->getValue(),
																					rightTemp,
																					isProcedureArgument(translationTable[rightArr->getName()]),
																					hardware));
			} else {
			  const auto &rightArrIndexVar = std::dynamic_pointer_cast<AstVariable>(rightArrIndex);

			  rightMoveNode.setInstruction(std::make_shared<BasicInstructionMovAV>(
				  translationTable[rightArr->getName()],
				  translationTable[rightArrIndexVar->getName()],
				  rightTemp, isProcedureArgument(translationTable[rightArr->getName()]),
				  hardware));
			}
		  }

		  rightMoveNode.addPredecessors(parentsIds);
		  instructions.push_back(rightMoveNode);
		  parentsIds = {rightMoveNode.getId()};

		  ControlFlowGraphNode mathNode;
		  mathNode.setInstruction(std::make_shared<BasicInstructionMathVV>(expressionType,
																		   leftTemp,
																		   rightTemp,
																		   tempResult,
																		   hardware));
		  mathNode.addPredecessors(parentsIds);
		  instructions.push_back(mathNode);
		  parentsIds = {mathNode.getId()};
		}

		ControlFlowGraphNode resultMoveNode;
		if (leftValue->getNodeType() == AstNodeType::VARIABLE) {
		  const auto &var = std::dynamic_pointer_cast<AstVariable>(leftValue);

		  resultMoveNode.setInstruction(std::make_shared<BasicInstructionMovVV>(tempResult,
																				translationTable[var->getName()],
																				hardware));
		} else {
		  const auto &arr = std::dynamic_pointer_cast<AstArray>(leftValue);
		  const auto &arrIndex = arr->getArgument();
		  if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
			const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);

			resultMoveNode.setInstruction(std::make_shared<BasicInstructionMovVKA>(tempResult,
																				   translationTable[arr->getName()],
																				   arrIndexNum->getValue(),
																				   isProcedureArgument(translationTable[arr->getName()]),
																				   hardware));
		  } else {
			const auto &arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);

			resultMoveNode.setInstruction(std::make_shared<BasicInstructionMovVA>(tempResult,
																				  translationTable[arr->getName()],
																				  translationTable[arrIndexVar->getName()],
																				  isProcedureArgument(translationTable[arr->getName()]),
																				  hardware));
		  }
		}

		resultMoveNode.addPredecessors(parentsIds);
		instructions.push_back(resultMoveNode);
		parentsIds = {resultMoveNode.getId()};
	  }
	  break;
	}
	case AstCommandType::IF: {
	  const auto &ifCommand = std::dynamic_pointer_cast<AstIf>(node);
	  const auto &condition = ifCommand->getCondition();
	  const auto &commands = ifCommand->getCommands()->getCommands();
	  const auto &elseCommands = ifCommand->getElseCommands()->getCommands();

	  const auto elseifLabel = hardware->getLabel(scopes.top(), "ifelse#", true);
	  const auto endifLabel = hardware->getLabel(scopes.top(), "endif#", false);

	  auto commandsInstructions = std::vector<ControlFlowGraphNode>();
	  auto elseCommandsInstructions = std::vector<ControlFlowGraphNode>();

	  auto condNodes = parseAstCondition(condition, elseifLabel);
	  condNodes.front().addPredecessors(parentsIds);
	  parentsIds = {condNodes.back().getId()};

	  for (size_t index = 0; index < commands.size(); index++) {
		parseAstCommand(commands[index], commandsInstructions);
	  }

	  instructions.insert(instructions.end(), condNodes.begin(), condNodes.end());
	  instructions.insert(instructions.end(), commandsInstructions.begin(), commandsInstructions.end());

	  if (elseCommands.empty()) {
		uint64_t elseIfLabelId = instructions.back().getId() + 1;
		if (currLabels.find(elseIfLabelId) == currLabels.end()) {
		  currLabels[elseIfLabelId] = {elseifLabel};
		} else {
		  currLabels[elseIfLabelId].push_back(elseifLabel);
		}

		parentsIds = {condNodes.back().getId(), instructions.back().getId()};
	  } else {
		ControlFlowGraphNode jumpEndNode;
		jumpEndNode.setInstruction(std::make_shared<BasicInstructionJump>(endifLabel, hardware));
		jumpEndNode.addPredecessors({instructions.back().getId()});

		uint64_t elseIfLabelId = jumpEndNode.getId() + 1;
		if (currLabels.find(elseIfLabelId) == currLabels.end()) {
		  currLabels[elseIfLabelId] = {elseifLabel};
		} else {
		  currLabels[elseIfLabelId].push_back(elseifLabel);
		}

		parentsIds = {condNodes.back().getId()};

		for (size_t index = 0; index < elseCommands.size(); index++) {
		  parseAstCommand(elseCommands[index], elseCommandsInstructions);
		}

		instructions.push_back(jumpEndNode);
		instructions.insert(instructions.end(), elseCommandsInstructions.begin(), elseCommandsInstructions.end());

		if (currLabels.find(instructions.back().getId() + 1) == currLabels.end()) {
		  currLabels[instructions.back().getId() + 1] = {endifLabel};
		} else {
		  currLabels[instructions.back().getId() + 1].push_back(endifLabel);
		}

		parentsIds = {jumpEndNode.getId(), elseCommandsInstructions.back().getId()};
	  }
	  break;
	}
	case AstCommandType::WHILE: {
	  const auto whileCommand = std::dynamic_pointer_cast<AstWhile>(node);
	  const auto &condition = whileCommand->getCondition();
	  const auto &commands = whileCommand->getCommands()->getCommands();

	  const auto beginLabel = hardware->getLabel(scopes.top(), "beginwhile#", true);
	  const auto endLabel = hardware->getLabel(scopes.top(), "endwhilejump#", true);

	  auto commandsInstructions = std::vector<ControlFlowGraphNode>();

	  auto condNodes = parseAstCondition(condition, endLabel);
	  condNodes.front().addPredecessors(parentsIds);

	  parentsIds = {condNodes.back().getId()};

	  for (size_t index = 0; index < commands.size(); index++) {
		parseAstCommand(commands[index], commandsInstructions);
	  }

	  ControlFlowGraphNode jumpNode;
	  jumpNode.setInstruction(std::make_shared<BasicInstructionJump>(beginLabel, hardware));
	  jumpNode.addPredecessors({commandsInstructions.back().getId()});

	  condNodes.front().addPredecessors({jumpNode.getId()});

	  instructions.insert(instructions.end(), condNodes.begin(), condNodes.end());
	  instructions.insert(instructions.end(), commandsInstructions.begin(), commandsInstructions.end());
	  instructions.push_back(jumpNode);

	  uint64_t beginLabelId = condNodes.front().getId();
	  if (currLabels.find(beginLabelId) == currLabels.end()) {
		currLabels[beginLabelId] = {beginLabel};
	  } else {
		currLabels[beginLabelId].push_back(beginLabel);
	  }

	  uint64_t endLabelId = instructions.back().getId() + 1;
	  if (currLabels.find(endLabelId) == currLabels.end()) {
		currLabels[endLabelId] = {endLabel};
	  } else {
		currLabels[endLabelId].push_back(endLabel);
	  }

	  parentsIds = {condNodes.front().getId(), jumpNode.getId()};
	  break;
	}
	case AstCommandType::REPEAT: {
	  const auto repeatCommand = std::dynamic_pointer_cast<AstRepeat>(node);
	  const auto &condition = repeatCommand->getCondition();
	  const auto &commands = repeatCommand->getCommands()->getCommands();

	  const auto repeatLabel = hardware->getLabel(scopes.top(), "beginrepeat#", true);

	  auto commandsInstructions = std::vector<ControlFlowGraphNode>();
	  for (size_t index = 0; index < commands.size(); index++) {
		parseAstCommand(commands[index], commandsInstructions);
	  }

	  auto condNodes = parseAstCondition(condition, repeatLabel);
	  condNodes.front().addPredecessors(parentsIds);

	  commandsInstructions.front().addPredecessors({condNodes.back().getId()});

	  instructions.insert(instructions.end(), commandsInstructions.begin(), commandsInstructions.end());
	  instructions.insert(instructions.end(), condNodes.begin(), condNodes.end());

	  uint64_t repeatLabelId = commandsInstructions.front().getId();
	  if (currLabels.find(repeatLabelId) == currLabels.end()) {
		currLabels[repeatLabelId] = {repeatLabel};
	  } else {
		currLabels[repeatLabelId].push_back(repeatLabel);
	  }

	  parentsIds = {condNodes.back().getId()};
	  break;
	}
	case AstCommandType::PROCEDURE_CALL: {
	  const auto &procedureCall = std::dynamic_pointer_cast<AstProcedureCall>(node);
	  const auto &procedureName = procedureCall->getName();
	  const auto &args = procedureCall->getArgsList()->getArgs();

	  auto argNames = std::vector<std::string>();
	  for (auto arg : args) {
		if (arg->getNodeType() == AstNodeType::VARIABLE) {
		  const auto &var = std::dynamic_pointer_cast<AstVariable>(arg);
		  argNames.emplace_back(translationTable[var->getName()]);
		} else {
		  const auto &arr = std::dynamic_pointer_cast<AstArray>(arg);

		  if (isProcedureArgument(translationTable[arr->getName()])) {
			argNames.emplace_back(translationTable[arr->getName()]);
		  } else {
			const auto arrBeginTemp = hardware->getTempRegister();
			const auto arrBeginNumber = hardware->getArrayAddress(translationTable[arr->getName()]);
			argNames.emplace_back(arrBeginTemp);

			ControlFlowGraphNode moveAddrNode;
			moveAddrNode.setInstruction(std::make_shared<BasicInstructionMovNV>(arrBeginNumber,
																				arrBeginTemp,
																				hardware));
			moveAddrNode.addPredecessors(parentsIds);
			instructions.push_back(moveAddrNode);
			parentsIds = {moveAddrNode.getId()};
		  }
		}
	  }

	  const auto functionRecordAddress = hardware->getFunctionRecordAddress(procedureName);

	  ControlFlowGraphNode pushArgsNode;
	  pushArgsNode.setInstruction(std::make_shared<BasicInstructionPush>(functionRecordAddress, argNames, hardware));
	  pushArgsNode.addPredecessors(parentsIds);
	  instructions.push_back(pushArgsNode);
	  parentsIds = {pushArgsNode.getId()};

	  usedProcedures.push_back(procedureName);
	  procedureCallsInProcedure[scopes.top()].push_back(procedureName);
	  ControlFlowGraphNode jumpNode;
	  jumpNode.setInstruction(std::make_shared<BasicInstructionJumpFun>(procedureName,
																		functionRecordAddress + args.size(),
																		hardware));
	  jumpNode.addPredecessors(parentsIds);
	  instructions.push_back(jumpNode);
	  parentsIds = {jumpNode.getId()};

	  std::string unused = Hardware::registerMap[RA].name;
	  ControlFlowGraphNode popArgsNode;
	  popArgsNode.setInstruction(std::make_shared<BasicInstructionPop>(functionRecordAddress, argNames,
																	   unused, hardware));
	  popArgsNode.addPredecessors(parentsIds);
	  instructions.push_back(popArgsNode);
	  parentsIds = {popArgsNode.getId()};

	  break;
	}
	case AstCommandType::UNDEFINED: {
	  std::cout << "Undefined command type" << std::endl;
	  break;
	}
  }
}

void Compiler::parseAstDeclarations(std::shared_ptr<AstDeclarations> node) {
  for (auto declaration : node->getDeclarations()) {
	const auto &name = declaration->getName();
	translationTable[name] = hardware->getGlobalLabel(scopes.top(), name);
	if (declaration->getNodeType() == AstNodeType::ARRAY) {
	  const auto &arr = std::dynamic_pointer_cast<AstArray>(declaration);
	  const auto &index = std::dynamic_pointer_cast<AstNumber>(arr->getArgument());
	  hardware->allocateArray(translationTable[name], index->getValue());
	}
  }
}

void Compiler::parseAstProcedureHeader(std::shared_ptr<AstProcedureHeader> node) {
  currProcedureArgs.clear();
  for (auto arg : node->getArgsDeclaration()->getArgs()) {
	const auto &name = arg->getName();
	translationTable[name] = hardware->getGlobalLabel(scopes.top(), name);
	currProcedureArgs.push_back(translationTable[name]);
  }
  proceduresArgs[node->getName()] = currProcedureArgs;

  size_t recordSize = node->getArgsDeclaration()->getArgs().size() + 1;
  hardware->allocateFunctionRecord(scopes.top(), recordSize);
}

std::vector<ControlFlowGraphNode> Compiler::parseAstCondition(std::shared_ptr<AstCondition> node,
															  const std::string &jumpLabel) {
  std::shared_ptr<AstValue> left;
  std::shared_ptr<AstValue> right;
  BasicInstructionConditionType jumpCondition;

  switch (node->getConditionType()) {
	case AstConditionType::GREATER: {
	  left = node->getLeft();
	  right = node->getRight();
	  jumpCondition = BasicInstructionConditionType::LESS_EQUAL;
	  break;
	}
	case AstConditionType::LESS: {
	  left = node->getRight();
	  right = node->getLeft();
	  jumpCondition = BasicInstructionConditionType::LESS_EQUAL;
	  break;
	}
	case AstConditionType::GREATER_EQUAL: {
	  left = node->getLeft();
	  right = node->getRight();
	  jumpCondition = BasicInstructionConditionType::LESS;
	  break;
	}
	case AstConditionType::LESS_EQUAL: {
	  left = node->getRight();
	  right = node->getLeft();
	  jumpCondition = BasicInstructionConditionType::LESS;
	  break;
	}
	case AstConditionType::EQUAL: {
	  left = node->getLeft();
	  right = node->getRight();
	  jumpCondition = BasicInstructionConditionType::NOT_EQUAL;
	  break;
	}
	case AstConditionType::NOT_EQUAL: {
	  left = node->getLeft();
	  right = node->getRight();
	  jumpCondition = BasicInstructionConditionType::EQUAL;
	  break;
	}
  }

  std::vector<ControlFlowGraphNode> result;

  if (left->getNodeType() == AstNodeType::NUMBER && right->getNodeType() == AstNodeType::NUMBER) {
	const auto &leftNum = std::dynamic_pointer_cast<AstNumber>(left);
	const auto &rightNum = std::dynamic_pointer_cast<AstNumber>(right);

	ControlFlowGraphNode condNode;
	condNode.setInstruction(std::make_shared<BasicInstructionCondJumpNN>(leftNum->getValue(),
																		 rightNum->getValue(),
																		 jumpCondition,
																		 jumpLabel,
																		 hardware));
	result.push_back(condNode);
  } else if (left->getNodeType() == AstNodeType::NUMBER) {
	const auto &leftNum = std::dynamic_pointer_cast<AstNumber>(left);
	const auto rightTmp = hardware->getTempRegister();
	auto moveNode = parseMoveLvalueToVariable(std::dynamic_pointer_cast<AstLeftValue>(right), rightTmp);

	ControlFlowGraphNode condNode;
	condNode.setInstruction(std::make_shared<BasicInstructionCondJumpNV>(leftNum->getValue(),
																		 rightTmp,
																		 jumpCondition,
																		 jumpLabel,
																		 hardware));
	result.push_back(moveNode);
	condNode.addPredecessors({moveNode.getId()});
	result.push_back(condNode);
  } else if (right->getNodeType() == AstNodeType::NUMBER) {
	const auto leftTmp = hardware->getTempRegister();
	auto moveNode = parseMoveLvalueToVariable(std::dynamic_pointer_cast<AstLeftValue>(left), leftTmp);
	const auto &rightNum = std::dynamic_pointer_cast<AstNumber>(right);

	ControlFlowGraphNode condNode;
	condNode.setInstruction(std::make_shared<BasicInstructionCondJumpVN>(leftTmp,
																		 rightNum->getValue(),
																		 jumpCondition,
																		 jumpLabel,
																		 hardware));
	result.push_back(moveNode);
	condNode.addPredecessors({moveNode.getId()});
	result.push_back(condNode);
  } else {
	const auto leftTmp = hardware->getTempRegister();
	auto moveLeftNode = parseMoveLvalueToVariable(std::dynamic_pointer_cast<AstLeftValue>(left), leftTmp);
	const auto rightTmp = hardware->getTempRegister();
	auto moveRightNode = parseMoveLvalueToVariable(std::dynamic_pointer_cast<AstLeftValue>(right), rightTmp);

	ControlFlowGraphNode condNode;
	condNode.setInstruction(std::make_shared<BasicInstructionCondJumpVV>(
		leftTmp,
		rightTmp,
		jumpCondition,
		jumpLabel,
		hardware));

	result.push_back(moveLeftNode);
	moveRightNode.addPredecessors({moveLeftNode.getId()});
	result.push_back(moveRightNode);
	condNode.addPredecessors({moveRightNode.getId()});
	result.push_back(condNode);
  }

  return result;
}

ControlFlowGraphNode Compiler::parseMoveLvalueToVariable(std::shared_ptr<AstLeftValue> lvalue,
														 std::string variableName) {
  if (lvalue->getNodeType() == AstNodeType::VARIABLE) {
	const auto &var = std::dynamic_pointer_cast<AstVariable>(lvalue);
	ControlFlowGraphNode moveNode;
	moveNode.setInstruction(std::make_shared<BasicInstructionMovVV>(translationTable[var->getName()],
																	variableName, hardware));
	return moveNode;
  }

  const auto &arr = std::dynamic_pointer_cast<AstArray>(lvalue);
  const auto &arrIndex = arr->getArgument();
  if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
	const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
	ControlFlowGraphNode moveNode;
	moveNode.setInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[arr->getName()],
																	 arrIndexNum->getValue(),
																	 variableName,
																	 isProcedureArgument(translationTable[arr->getName()]),
																	 hardware));
	return moveNode;
  }

  const auto &arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
  ControlFlowGraphNode moveNode;
  moveNode.setInstruction(std::make_shared<BasicInstructionMovAV>(translationTable[arr->getName()],
																  translationTable[arrIndexVar->getName()],
																  variableName,
																  isProcedureArgument(translationTable[arr->getName()]),
																  hardware));
  return moveNode;
}

bool Compiler::isProcedureArgument(const std::string &name) {
  return std::find(currProcedureArgs.begin(), currProcedureArgs.end(), name) != currProcedureArgs.end();
}

bool Compiler::pasteProcedureEfficiency(const std::string &name) {
  size_t functionEstimatedSize{0};

  std::stack<std::string> usedProceduresInProcedure;
  usedProceduresInProcedure.push(name);
  while (!usedProceduresInProcedure.empty()) {
	auto proc = usedProceduresInProcedure.top();
	usedProceduresInProcedure.pop();

	for (const auto &node : firstControlFlowGraph.getNodes(proc)) {
	  functionEstimatedSize += node.getInstruction()->estimateMachineCodeSize();
	}

	if (procedureCallsInProcedure.find(proc) != procedureCallsInProcedure.end()) {
	  for (auto call : procedureCallsInProcedure[proc]) {
		usedProceduresInProcedure.push(call);
	  }
	}
  }
  std::cout << "ESTIMATED SIZE " << name << ": " << functionEstimatedSize << std::endl;
  if(AstProcedureCall::getFunctionCallsCounter().at(name) == 1) {
	std::cout << "FAST PASTE " << name << std::endl;
	return true;
  }
  return functionEstimatedSize <= 200 * (proceduresArgs[name].size());
//  return false;
}