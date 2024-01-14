#include <iostream>

#include "../../include/core/Compiler.hpp"

Compiler::Compiler() {}

Compiler::Compiler(std::shared_ptr<AstNode> ast) : ast(ast), hardware(std::make_shared<Hardware>()) {}

void Compiler::generateMachineCode(std::ofstream& outputFile) {
  convertToControlFlowGraph();
  optimizeBasicInstructions();
  expandBasicInstructions();
  calculateVariablesLiveRanges();
  optimizeMachineCode();
  generateOutput(outputFile);
}

void Compiler::generateMachineCodeWithDebug(std::ofstream& astFile, std::ofstream& basicBlocksFile,
                                            std::ofstream& machineCodeFile, std::ofstream& outputFile) {
  convertToControlFlowGraph();
  optimizeBasicInstructions();
  expandBasicInstructions();
  calculateVariablesLiveRanges();
  optimizeMachineCode();
  generateOutput(outputFile);

  ast->print(astFile, 0);
  controlFlowGraph.printGraph(basicBlocksFile);
  controlFlowGraph.printInstructions(machineCodeFile);

  //  for (const auto& instruction : instructions) {
  //    instruction->print(instructionsFile);
  //    instructionsFile << std::endl;
  //  }
  //
  //  for (const auto& instruction : machineCode) {
  //    const auto& opcode = Hardware::instructionMap[instruction.first].opcode;
  //    const auto& argument = instruction.second;
  //
  //    if (argument.find("begin#") != std::string::npos) {
  //      partialMachineCodeFile << std::endl;
  //    }
  //    partialMachineCodeFile << opcode << " " << argument << std::endl;
  //    if (argument.find("end#") != std::string::npos) {
  //      partialMachineCodeFile << std::endl;
  //    }
  //  }
}

void Compiler::convertToControlFlowGraph() {
  const std::shared_ptr<AstProgram> program = std::dynamic_pointer_cast<AstProgram>(ast);
  const std::shared_ptr<AstMain>& main = program->getMain();
  const std::shared_ptr<AstProcedures>& procedures = program->getProcedures();

  std::pair<std::string, std::vector<ControlFlowGraphNode>> blockInstructions;

  parseAstMain(main, blockInstructions);
  controlFlowGraph.addSubgraph(blockInstructions.first, blockInstructions.second);

  for (const auto& procedure : procedures->getProcedures()) {
    parseAstProcedure(procedure, blockInstructions);
    controlFlowGraph.addSubgraph(blockInstructions.first, blockInstructions.second);
  }

  controlFlowGraph.initializeSuccessorsBasedOnPredecessors();
}

void Compiler::calculateVariablesLiveRanges() {
  controlFlowGraph.calculateVariablesLiveRanges();

  const auto& varLiveIn = controlFlowGraph.getLiveInVariables();
  const auto& varLiveOut = controlFlowGraph.getLiveOutVariables();
  const auto& varLiveNow = controlFlowGraph.getLiveNowVariables();
  const auto& definedVariables = controlFlowGraph.getDefinedVariables();
  const auto& usedVariables = controlFlowGraph.getUsedVariables();

  registersLinearScan = RegistersLinearScan{hardware};
  for (const auto& var : varLiveIn) {
    registersLinearScan.addVariableLiveInfo(var.second, var.first);
  }
  for (const auto& var : varLiveOut) {
    registersLinearScan.addVariableLiveInfo(var.second, var.first);
  }
  for (const auto& var : varLiveNow) {
    registersLinearScan.addVariableLiveInfo(var.second, var.first);
  }

  for (const auto& var : definedVariables) {
    registersLinearScan.addVariableUsageInfo(var.second, var.first, UsageType::LVAL);
  }

  for (const auto& var : usedVariables) {
    registersLinearScan.addVariableUsageInfo(var.second, var.first, UsageType::RVAL);
  }

  registersLinearScan.createRanges();
  registersLinearScan.allocateRegisters();
}

void Compiler::optimizeBasicInstructions() {}

void Compiler::expandBasicInstructions() {
  controlFlowGraph.expandBasicInstructions();
}

void Compiler::optimizeMachineCode() {}

void Compiler::generateOutput(std::ofstream& outputFile) {
  const auto nodes = controlFlowGraph.getInstructions("main");

  for (const auto& node : nodes) {
    const auto memoryInteractions = registersLinearScan.getMemoryInteractions(node.getId());
    for (const auto& memoryInteraction : memoryInteractions) {
      const auto& opcode = memoryInteraction.first;
      const auto& argument = memoryInteraction.second;
      if (opcode == HardwareInstruction::LOAD) {
        //outputFile << "-->LOAD " << argument.first << " " << argument.second << std::endl;
        const auto generateNumber = Utils::generateNumber(argument.second, Hardware::registerMap[Hardware::accumulator].name);
        for (const auto& instruction : generateNumber) {
          outputFile << Hardware::instructionMap[instruction.first].opcode << " " << instruction.second << std::endl;
        }
        outputFile << "LOAD " << Hardware::registerMap[Hardware::accumulator].name << std::endl;
        outputFile << "PUT " << argument.first << std::endl;
      }
      else if (opcode == HardwareInstruction::STORE) {
        //outputFile << "-->STORE " << argument.first << " " << argument.second << std::endl;
        const auto generateNumber = Utils::generateNumber(argument.second, argument.first);
        outputFile << "GET " << argument.first << std::endl;
        for (const auto& instruction : generateNumber) {
          outputFile << Hardware::instructionMap[instruction.first].opcode << " " << instruction.second << std::endl;
        }
        outputFile << "STORE " << argument.first << std::endl;
      }
    }

    const auto& instructions = node.getInstructions();
    for (const auto& instruction : instructions) {
      for (const auto& machineCode : instruction->getMachineCode()) {
        const auto& opcode = Hardware::instructionMap[machineCode.first].opcode;
        const auto& argument = machineCode.second;

        outputFile << opcode << " " << registersLinearScan.getVariableRegister(argument, node.getId()) << std::endl;
      }
    }
  }
}

void Compiler::parseAstMain(std::shared_ptr<AstMain> node,
                            std::pair<std::string, std::vector<ControlFlowGraphNode>>& instructions) {

  instructions.first = "main";
  instructions.second.clear();

  scopes.push("main");

  parseAstDeclarations(node->getDeclarations());

  parentsIds.clear();

  const auto& commands = node->getCommands()->getCommands();
  auto commandsInstructions = std::vector<ControlFlowGraphNode>();
  for (size_t index = 0; index < commands.size(); index++) {
    parseAstCommand(commands[index], commandsInstructions);
  }

  ControlFlowGraphNode haltNode;
  haltNode.addInstruction(std::make_shared<BasicInstructionHalt>(hardware));
  haltNode.addPredecessors(parentsIds);

  instructions.second.insert(instructions.second.end(), commandsInstructions.begin(), commandsInstructions.end());
  instructions.second.emplace_back(haltNode);

  scopes.pop();
}

void Compiler::parseAstProcedure(std::shared_ptr<AstProcedure> node,
                                 std::pair<std::string, std::vector<ControlFlowGraphNode>>& instructions) {
  const auto& procedureName = node->getHeader()->getName();

  instructions.first = procedureName;
  instructions.second.clear();

  scopes.push(procedureName);

  parseAstProcedureHeader(node->getHeader());
  parseAstDeclarations(node->getDeclarations());

  parentsIds.clear();

  const auto& commands = node->getCommands()->getCommands();
  auto commandsInstructions = std::vector<ControlFlowGraphNode>();
  for (size_t index = 0; index < commands.size(); index++) {
    parseAstCommand(commands[index], commandsInstructions);
  }

  instructions.second.insert(instructions.second.end(), commandsInstructions.begin(), commandsInstructions.end());

  scopes.pop();
}

void Compiler::parseAstCommand(std::shared_ptr<AstCommand> node, std::vector<ControlFlowGraphNode>& instructions) {
  switch (node->getCommandType()) {
    case AstCommandType::READ: {
      const auto& read = std::dynamic_pointer_cast<AstRead>(node);
      const auto& leftValue = read->getLvalue();
      if (leftValue->getNodeType() == AstNodeType::VARIABLE) {
        const auto& var = std::dynamic_pointer_cast<AstVariable>(leftValue);

        ControlFlowGraphNode node;
        node.addInstruction(std::make_shared<BasicInstructionRead>(translationTable[var->getName()], hardware));
        node.addPredecessors(parentsIds);
        instructions.push_back(node);
        parentsIds = {node.getId()};
      }
      else {
        const auto& arr = std::dynamic_pointer_cast<AstArray>(leftValue);
        const auto& arrIndex = arr->getArgument();
        if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
          const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
          const auto& temp = hardware->getTempRegister();

          ControlFlowGraphNode node;
          node.addInstruction(std::make_shared<BasicInstructionRead>(temp, hardware));
          node.addInstruction(std::make_shared<BasicInstructionMovVKA>(temp, translationTable[arr->getName()],
                                                                       arrIndexNum->getValue(), hardware));
          node.addPredecessors(parentsIds);
          instructions.push_back(node);
          parentsIds = {node.getId()};
        }
        else {
          const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
          const auto& temp = hardware->getTempRegister();

          ControlFlowGraphNode node;
          node.addInstruction(std::make_shared<BasicInstructionRead>(temp, hardware));
          node.addInstruction(std::make_shared<BasicInstructionMovVA>(temp, translationTable[arr->getName()],
                                                                      translationTable[arrIndexVar->getName()], hardware));
          node.addPredecessors(parentsIds);
          instructions.push_back(node);
          parentsIds = {node.getId()};
        }
      }
      break;
    }
    case AstCommandType::WRITE: {
      const auto& write = std::dynamic_pointer_cast<AstWrite>(node);
      const auto& value = write->getValue();
      if (value->getNodeType() == AstNodeType::NUMBER) {
        const auto& num = std::dynamic_pointer_cast<AstNumber>(value);
        const auto& acc = Hardware::registerMap[Hardware::accumulator].name;

        ControlFlowGraphNode node;
        node.addInstruction(std::make_shared<BasicInstructionMovNV>(num->getValue(), acc, hardware));
        node.addInstruction(std::make_shared<BasicInstructionWrite>(acc, hardware));
        node.addPredecessors(parentsIds);
        instructions.push_back(node);
        parentsIds = {node.getId()};
      }
      else if (value->getNodeType() == AstNodeType::VARIABLE) {
        const auto& var = std::dynamic_pointer_cast<AstVariable>(value);

        ControlFlowGraphNode node;
        node.addInstruction(std::make_shared<BasicInstructionWrite>(translationTable[var->getName()], hardware));
        node.addPredecessors(parentsIds);
        instructions.push_back(node);
        parentsIds = {node.getId()};
      }
      else {
        const auto& arr = std::dynamic_pointer_cast<AstArray>(value);
        const auto& arrIndex = arr->getArgument();
        if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
          const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
          const auto& temp = hardware->getTempRegister();

          ControlFlowGraphNode node;
          node.addInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[arr->getName()], arrIndexNum->getValue(),
                                                                       temp, hardware));
          node.addInstruction(std::make_shared<BasicInstructionWrite>(temp, hardware));
          node.addPredecessors(parentsIds);
          instructions.push_back(node);
          parentsIds = {node.getId()};
        }
        else {
          const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
          const auto& temp = hardware->getTempRegister();

          ControlFlowGraphNode node;
          node.addInstruction(std::make_shared<BasicInstructionMovAV>(translationTable[arr->getName()],
                                                                      translationTable[arrIndexVar->getName()], temp, hardware));
          node.addInstruction(std::make_shared<BasicInstructionWrite>(temp, hardware));
          node.addPredecessors(parentsIds);
          instructions.push_back(node);
          parentsIds = {node.getId()};
        }
      }
      break;
    }
    case AstCommandType::ASSIGNMENT: {
      const auto& assignment = std::dynamic_pointer_cast<AstAssignment>(node);
      const auto& leftValue = assignment->getLvalue();
      const auto& expression = assignment->getExpression();
      const auto& result = leftValue->getNodeType() == AstNodeType::VARIABLE ? translationTable[leftValue->getName()]
                                                                             : hardware->getTempRegister();

      if (expression->getRight() == nullptr) {
        ControlFlowGraphNode node;

        const auto& newValue = expression->getLeft();
        if (newValue->getNodeType() == AstNodeType::NUMBER) {
          const auto& newNum = std::dynamic_pointer_cast<AstNumber>(newValue);

          node.addInstruction(std::make_shared<BasicInstructionMovNV>(newNum->getValue(), result, hardware));
        }
        else if (newValue->getNodeType() == AstNodeType::VARIABLE) {
          const auto& newVar = std::dynamic_pointer_cast<AstVariable>(newValue);

          node.addInstruction(std::make_shared<BasicInstructionMovVV>(translationTable[newVar->getName()], result, hardware));
        }
        else {
          const auto& newArr = std::dynamic_pointer_cast<AstArray>(newValue);
          const auto& newArrIndex = newArr->getArgument();
          if (newArrIndex->getNodeType() == AstNodeType::NUMBER) {

            node.addInstruction(std::make_shared<BasicInstructionMovKAV>(
                translationTable[newArr->getName()], std::dynamic_pointer_cast<AstNumber>(newArrIndex)->getValue(), result,
                hardware));
          }
          else {
            node.addInstruction(std::make_shared<BasicInstructionMovAV>(
                translationTable[newArr->getName()],
                translationTable[std::dynamic_pointer_cast<AstVariable>(newArrIndex)->getName()], result, hardware));
          }
        }

        if (leftValue->getNodeType() == AstNodeType::VARIABLE) {
          const auto& var = std::dynamic_pointer_cast<AstVariable>(leftValue);

          node.addInstruction(std::make_shared<BasicInstructionMovVV>(result, translationTable[var->getName()], hardware));
        }
        else {
          const auto& arr = std::dynamic_pointer_cast<AstArray>(leftValue);
          const auto& arrIndex = arr->getArgument();
          if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
            const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);

            node.addInstruction(std::make_shared<BasicInstructionMovVKA>(result, translationTable[arr->getName()],
                                                                         arrIndexNum->getValue(), hardware));
          }
          else {
            const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);

            node.addInstruction(std::make_shared<BasicInstructionMovVA>(result, translationTable[arr->getName()],
                                                                        translationTable[arrIndexVar->getName()], hardware));
          }
        }

        node.addPredecessors(parentsIds);
        instructions.push_back(node);
        parentsIds = {node.getId()};
      }
      else {
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
        const auto& leftOperand = expression->getLeft();
        const auto& rightOperand = expression->getRight();

        ControlFlowGraphNode node;

        if (leftOperand->getNodeType() == AstNodeType::NUMBER && rightOperand->getNodeType() == AstNodeType::NUMBER) {
          const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(leftOperand);
          const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(rightOperand);

          node.addInstruction(std::make_shared<BasicInstructionMathNN>(expressionType, leftNum->getValue(), rightNum->getValue(),
                                                                       result, hardware));
        }
        else if (leftOperand->getNodeType() == AstNodeType::NUMBER) {
          const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(leftOperand);

          if (rightOperand->getNodeType() == AstNodeType::VARIABLE) {
            const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(rightOperand);

            node.addInstruction(std::make_shared<BasicInstructionMathNV>(
                expressionType, leftNum->getValue(), translationTable[rightVar->getName()], result, hardware));
          }
          else {
            const auto& rightArr = std::dynamic_pointer_cast<AstArray>(rightOperand);
            const auto& rightArrIndex = rightArr->getArgument();
            const auto& temp = hardware->getTempRegister();
            if (rightArrIndex->getNodeType() == AstNodeType::NUMBER) {
              const auto rightArrIndexNum = std::dynamic_pointer_cast<AstNumber>(rightArrIndex);

              node.addInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[rightArr->getName()],
                                                                           rightArrIndexNum->getValue(), temp, hardware));
            }
            else {
              const auto& rightArrIndexVar = std::dynamic_pointer_cast<AstVariable>(rightArrIndex);

              node.addInstruction(std::make_shared<BasicInstructionMovAV>(
                  translationTable[rightArr->getName()], translationTable[rightArrIndexVar->getName()], temp, hardware));
            }

            node.addInstruction(
                std::make_shared<BasicInstructionMathNV>(expressionType, leftNum->getValue(), temp, result, hardware));
          }
        }
        else if (rightOperand->getNodeType() == AstNodeType::NUMBER) {
          const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(rightOperand);

          if (leftOperand->getNodeType() == AstNodeType::VARIABLE) {
            const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(leftOperand);

            node.addInstruction(std::make_shared<BasicInstructionMathVN>(expressionType, translationTable[leftVar->getName()],
                                                                         rightNum->getValue(), result, hardware));
          }
          else {
            const auto& leftArr = std::dynamic_pointer_cast<AstArray>(leftOperand);
            const auto& leftArrIndex = leftArr->getArgument();
            const auto& temp = hardware->getTempRegister();
            if (leftArrIndex->getNodeType() == AstNodeType::NUMBER) {
              const auto leftArrIndexNum = std::dynamic_pointer_cast<AstNumber>(leftArrIndex);

              node.addInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[leftArr->getName()],
                                                                           leftArrIndexNum->getValue(), temp, hardware));
            }
            else {
              const auto& leftArrIndexVar = std::dynamic_pointer_cast<AstVariable>(leftArrIndex);

              node.addInstruction(std::make_shared<BasicInstructionMovAV>(
                  translationTable[leftArr->getName()], translationTable[leftArrIndexVar->getName()], temp, hardware));
            }

            node.addInstruction(
                std::make_shared<BasicInstructionMathVN>(expressionType, temp, rightNum->getValue(), result, hardware));
          }
        }
        else {
          const auto& temp1 = hardware->getTempRegister();
          if (leftOperand->getNodeType() == AstNodeType::VARIABLE) {
            const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(leftOperand);

            node.addInstruction(std::make_shared<BasicInstructionMovVV>(translationTable[leftVar->getName()], temp1, hardware));
          }
          else {
            const auto& leftArr = std::dynamic_pointer_cast<AstArray>(leftOperand);
            const auto& leftArrIndex = leftArr->getArgument();
            if (leftArrIndex->getNodeType() == AstNodeType::NUMBER) {
              const auto leftArrIndexNum = std::dynamic_pointer_cast<AstNumber>(leftArrIndex);

              node.addInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[leftArr->getName()],
                                                                           leftArrIndexNum->getValue(), temp1, hardware));
            }
            else {
              const auto& leftArrIndexVar = std::dynamic_pointer_cast<AstVariable>(leftArrIndex);

              node.addInstruction(std::make_shared<BasicInstructionMovAV>(
                  translationTable[leftArr->getName()], translationTable[leftArrIndexVar->getName()], temp1, hardware));
            }
          }

          const auto& temp2 = hardware->getTempRegister();
          if (rightOperand->getNodeType() == AstNodeType::VARIABLE) {
            const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(rightOperand);

            node.addInstruction(std::make_shared<BasicInstructionMovVV>(translationTable[rightVar->getName()], temp2, hardware));
          }
          else {
            const auto& rightArr = std::dynamic_pointer_cast<AstArray>(rightOperand);
            const auto& rightArrIndex = rightArr->getArgument();
            if (rightArrIndex->getNodeType() == AstNodeType::NUMBER) {
              const auto rightArrIndexNum = std::dynamic_pointer_cast<AstNumber>(rightArrIndex);

              node.addInstruction(std::make_shared<BasicInstructionMovKAV>(translationTable[rightArr->getName()],
                                                                           rightArrIndexNum->getValue(), temp2, hardware));
            }
            else {
              const auto& rightArrIndexVar = std::dynamic_pointer_cast<AstVariable>(rightArrIndex);

              node.addInstruction(std::make_shared<BasicInstructionMovAV>(
                  translationTable[rightArr->getName()], translationTable[rightArrIndexVar->getName()], temp2, hardware));
            }
          }

          node.addInstruction(std::make_shared<BasicInstructionMathVV>(expressionType, temp1, temp2, result, hardware));
        }

        if (leftValue->getNodeType() == AstNodeType::VARIABLE) {
          const auto& var = std::dynamic_pointer_cast<AstVariable>(leftValue);

          node.addInstruction(std::make_shared<BasicInstructionMovVV>(result, translationTable[var->getName()], hardware));
        }
        else {
          const auto& arr = std::dynamic_pointer_cast<AstArray>(leftValue);
          const auto& arrIndex = arr->getArgument();
          if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
            const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);

            node.addInstruction(std::make_shared<BasicInstructionMovVKA>(result, translationTable[arr->getName()],
                                                                         arrIndexNum->getValue(), hardware));
          }
          else {
            const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);

            node.addInstruction(std::make_shared<BasicInstructionMovVA>(result, translationTable[arr->getName()],
                                                                        translationTable[arrIndexVar->getName()], hardware));
          }
        }

        node.addPredecessors(parentsIds);
        instructions.push_back(node);
        parentsIds = {node.getId()};
      }
      break;
    }
    case AstCommandType::IF: {
      const auto& ifCommand = std::dynamic_pointer_cast<AstIf>(node);
      const auto& condition = ifCommand->getCondition();
      const auto& commands = ifCommand->getCommands()->getCommands();
      const auto& elseCommands = ifCommand->getElseCommands()->getCommands();

      const auto jumpLabel = hardware->getLabel(scopes.top(), "ifjump#", true);
      //const auto endifLabel = hardware->getLabel(scopes.top(), "endif#", false);

      auto commandsInstructions = std::vector<ControlFlowGraphNode>();
      auto elseCommandsInstructions = std::vector<ControlFlowGraphNode>();

      ControlFlowGraphNode condNode = parseAstCondition(condition, jumpLabel);
      condNode.addPredecessors(parentsIds);
      parentsIds = {condNode.getId()};

      for (size_t index = 0; index < commands.size(); index++) {
        parseAstCommand(commands[index], commandsInstructions);
      }

      parentsIds = {condNode.getId()};

      for (size_t index = 0; index < elseCommands.size(); index++) {
        parseAstCommand(elseCommands[index], elseCommandsInstructions);
      }

      instructions.push_back(condNode);
      instructions.insert(instructions.end(), commandsInstructions.begin(), commandsInstructions.end());
      instructions.insert(instructions.end(), elseCommandsInstructions.begin(), elseCommandsInstructions.end());

      if (elseCommandsInstructions.empty()) {
        parentsIds = {condNode.getId(), commandsInstructions.back().getId()};
      }
      else {
        parentsIds = {commandsInstructions.back().getId(), elseCommandsInstructions.back().getId()};
      }
      labels[jumpLabel] = commandsInstructions.back().getId() + 1;
      break;
    }
    case AstCommandType::WHILE: {
      const auto whileCommand = std::dynamic_pointer_cast<AstWhile>(node);
      const auto& condition = whileCommand->getCondition();
      const auto& commands = whileCommand->getCommands()->getCommands();

      const auto beginLabel = hardware->getLabel(scopes.top(), "beginwhile#", true);
      const auto endLabel = hardware->getLabel(scopes.top(), "endwhilejump#", true);

      auto commandsInstructions = std::vector<ControlFlowGraphNode>();

      ControlFlowGraphNode condNode = parseAstCondition(condition, endLabel);
      condNode.addPredecessors(parentsIds);

      parentsIds = {condNode.getId()};

      for (size_t index = 0; index < commands.size(); index++) {
        parseAstCommand(commands[index], commandsInstructions);
      }

      ControlFlowGraphNode jumpNode;
      jumpNode.addInstruction(std::make_shared<BasicInstructionJump>(beginLabel, hardware));
      jumpNode.addPredecessors({commandsInstructions.back().getId()});

      condNode.addPredecessors({jumpNode.getId()});

      instructions.push_back(condNode);
      instructions.insert(instructions.end(), commandsInstructions.begin(), commandsInstructions.end());
      instructions.push_back(jumpNode);

      parentsIds = {condNode.getId(), jumpNode.getId()};
      labels[beginLabel] = condNode.getId();
      labels[endLabel] = jumpNode.getId() + 1;
      break;
    }
    case AstCommandType::REPEAT: {
      const auto repeatCommand = std::dynamic_pointer_cast<AstRepeat>(node);
      const auto& condition = repeatCommand->getCondition();
      const auto& commands = repeatCommand->getCommands()->getCommands();

      const auto repeatLabel = hardware->getLabel(scopes.top(), "beginrepeat#", true);

      auto commandsInstructions = std::vector<ControlFlowGraphNode>();
      for (size_t index = 0; index < commands.size(); index++) {
        parseAstCommand(commands[index], commandsInstructions);
      }

      ControlFlowGraphNode condNode = parseAstCondition(condition, repeatLabel);
      condNode.addPredecessors(parentsIds);

      commandsInstructions.front().addPredecessors({condNode.getId()});

      instructions.insert(instructions.end(), commandsInstructions.begin(), commandsInstructions.end());
      instructions.push_back(condNode);

      parentsIds = {condNode.getId()};
      labels[repeatLabel] = commandsInstructions.front().getId();
      break;
    }
    case AstCommandType::PROCEDURE_CALL: {
      const auto& procedureCall = std::dynamic_pointer_cast<AstProcedureCall>(node);
      const auto& procedureName = procedureCall->getName();
      //      const auto& args = procedureCall->getArgsList()->getArgs();
      //      auto argNames = std::vector<std::string>();
      //      for (auto arg : args) {
      //        const auto& var = std::dynamic_pointer_cast<AstVariable>(arg);
      //        argNames.emplace_back(translationTable[var->getName()]);
      //      }
      //      instructions.emplace_back(std::make_shared<BasicInstructionPush>(hardware->getStackOffset(), argNames, hardware));
      //      hardware->incrementStackOffset(args.size());

      const auto& beginLabel = hardware->getGlobalLabel(procedureName, "begin#");

      ControlFlowGraphNode jumpNode;
      jumpNode.addInstruction(std::make_shared<BasicInstructionJump>(beginLabel, hardware));
      jumpNode.addPredecessors(parentsIds);

      instructions.push_back(jumpNode);

      parentsIds = {jumpNode.getId()};
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
    const auto& name = declaration->getName();
    translationTable[name] = hardware->getGlobalLabel(scopes.top(), name);
    if (declaration->getNodeType() == AstNodeType::ARRAY) {
      const auto& arr = std::dynamic_pointer_cast<AstArray>(declaration);
      const auto& index = std::dynamic_pointer_cast<AstNumber>(arr->getArgument());
      hardware->allocateArray(translationTable[name], index->getValue());
    }
  }
}

void Compiler::parseAstProcedureHeader(std::shared_ptr<AstProcedureHeader> node) {
  for (auto arg : node->getArgsDeclaration()->getArgs()) {
    const auto& name = arg->getName();
    translationTable[name] = hardware->getGlobalLabel(scopes.top(), name);
  }
}

ControlFlowGraphNode Compiler::parseAstCondition(std::shared_ptr<AstCondition> node, const std::string& jumpLabel) {
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

  ControlFlowGraphNode result;

  if (left->getNodeType() == AstNodeType::NUMBER && right->getNodeType() == AstNodeType::NUMBER) {
    const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(left);
    const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(right);

    result.addInstruction(std::make_shared<BasicInstructionCondJumpNN>(leftNum->getValue(), rightNum->getValue(), jumpCondition,
                                                                       jumpLabel, hardware));
  }
  else if (left->getNodeType() == AstNodeType::NUMBER) {
    const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(left);
    const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(right);

    result.addInstruction(std::make_shared<BasicInstructionCondJumpNV>(leftNum->getValue(), translationTable[rightVar->getName()],
                                                                       jumpCondition, jumpLabel, hardware));
  }
  else if (right->getNodeType() == AstNodeType::NUMBER) {
    const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(left);
    const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(right);

    result.addInstruction(std::make_shared<BasicInstructionCondJumpVN>(translationTable[leftVar->getName()], rightNum->getValue(),
                                                                       jumpCondition, jumpLabel, hardware));
  }
  else {
    const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(left);
    const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(right);

    result.addInstruction(std::make_shared<BasicInstructionCondJumpVV>(
        translationTable[leftVar->getName()], translationTable[rightVar->getName()], jumpCondition, jumpLabel, hardware));
  }

  return result;
}
