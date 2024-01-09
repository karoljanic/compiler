#include <iostream>

#include "../../include/core/Compiler.hpp"

Compiler::Compiler() {}

Compiler::Compiler(std::shared_ptr<AstNode> ast) : ast(ast), hardware(std::make_shared<Hardware>()) {}

void Compiler::generateMachineCode(std::ofstream& /*outputFile*/) {
  convertToBasicInstructions(ast);
  optimizeBasicInstructions();
  expandBasicInstructions();
  optimizeMachineCode();
  assignRegistersAndMemory();
  optimizeFinalMachineCode();
  assignLabels();

  std::cout << std::endl << "Generating machine code..." << std::endl << std::endl;
}

void Compiler::generateMachineCodeWithDebug(std::ofstream& basicInstructionsFile, std::ofstream& partialMachineCodeFile,
                                            std::ofstream& /*finalMachineCodeFile*/, std::ofstream& /*outputFile*/) {
  convertToBasicInstructions(ast);
  optimizeBasicInstructions();
  expandBasicInstructions();
  optimizeMachineCode();
  assignRegistersAndMemory();
  optimizeFinalMachineCode();
  assignLabels();

  for (const auto& instruction : basicInstructions) {
    instruction->print(basicInstructionsFile);
    basicInstructionsFile << std::endl;
  }

  for (const auto& instruction : machineCode) {
    const auto& opcode = Hardware::instructionMap[instruction.first].opcode;
    const auto& argument = instruction.second;

    if (argument.find("begin#") != std::string::npos) {
      partialMachineCodeFile << std::endl;
    }
    partialMachineCodeFile << opcode << " " << argument << std::endl;
    if (argument.find("end#") != std::string::npos) {
      partialMachineCodeFile << std::endl;
    }
  }
}

void Compiler::convertToBasicInstructions(const std::shared_ptr<AstNode>& ast) {
  if (ast == nullptr) {
    return;
  }

  switch (ast->getNodeType()) {
    case AstNodeType::PROGRAM: {
      const auto& program = std::dynamic_pointer_cast<AstProgram>(ast);
      convertToBasicInstructions(program->getMain());
      convertToBasicInstructions(program->getProcedures());
      break;
    }
    case AstNodeType::PROCEDURES: {
      const auto& procedures = std::dynamic_pointer_cast<AstProcedures>(ast);
      for (auto procedure : procedures->getProcedures()) {
        convertToBasicInstructions(procedure);
      }
      break;
    }
    case AstNodeType::PROCEDURE: {
      const auto& procedure = std::dynamic_pointer_cast<AstProcedure>(ast);
      const auto& procedureName = procedure->getHeader()->getName();
      scopes.push(procedureName);
      const auto& beginLabel = hardware->getGlobalLabel(procedureName, "begin#");
      const auto& endLabel = hardware->getGlobalLabel(procedureName, "end#");
      basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(beginLabel, hardware));
      convertToBasicInstructions(procedure->getHeader());
      convertToBasicInstructions(procedure->getDeclarations());
      std::vector<std::string> args;
      for (auto arg : procedure->getHeader()->getArgsDeclaration()->getArgs()) {
        args.emplace_back(translationTable[arg->getName()]);
      }
      std::reverse(args.begin(), args.end());
      basicInstructions.emplace_back(std::make_shared<BasicInstructionPop>(hardware->getStackOffset(), args, hardware));
      convertToBasicInstructions(procedure->getCommands());
      basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(endLabel, hardware));
      scopes.pop();
      break;
    }
    case AstNodeType::MAIN: {
      const auto& main = std::dynamic_pointer_cast<AstMain>(ast);
      scopes.push("main");
      const auto& beginLabel = hardware->getGlobalLabel("main", "begin#");
      const auto& endLabel = hardware->getGlobalLabel("main", "end#");
      basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(beginLabel, hardware));
      convertToBasicInstructions(main->getDeclarations());
      convertToBasicInstructions(main->getCommands());
      basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(endLabel, hardware));
      scopes.pop();
      break;
    }
    case AstNodeType::PROCEDURE_HEADER: {
      const auto& procedureHeader = std::dynamic_pointer_cast<AstProcedureHeader>(ast);
      const auto& args = procedureHeader->getArgsDeclaration()->getArgs();
      for (auto arg : args) {
        const auto& name = arg->getName();
        translationTable[name] = hardware->getGlobalLabel(scopes.top(), name);
      }
      break;
    }
    case AstNodeType::DECLARATIONS: {
      const auto& declarations = std::dynamic_pointer_cast<AstDeclarations>(ast);
      for (auto declaration : declarations->getDeclarations()) {
        const auto& name = declaration->getName();
        translationTable[name] = hardware->getGlobalLabel(scopes.top(), name);
        if (declaration->getNodeType() == AstNodeType::ARRAY) {
          const auto& arr = std::dynamic_pointer_cast<AstArray>(declaration);
          const auto& index = std::dynamic_pointer_cast<AstNumber>(arr->getArgument());
          hardware->allocateArray(translationTable[name], index->getValue());
        }
      }
      break;
    }
    case AstNodeType::COMMANDS: {
      const auto& commands = std::dynamic_pointer_cast<AstCommands>(ast);
      for (auto command : commands->getCommands()) {
        convertToBasicInstructions(command);
      }
      break;
    }
    case AstNodeType::COMMAND: {
      const auto& command = std::dynamic_pointer_cast<AstCommand>(ast);
      switch (command->getCommandType()) {
        case AstCommandType::READ: {
          const auto& read = std::dynamic_pointer_cast<AstRead>(command);
          const auto& leftValue = read->getLvalue();
          if (leftValue->getNodeType() == AstNodeType::VARIABLE) {
            const auto& var = std::dynamic_pointer_cast<AstVariable>(leftValue);
            basicInstructions.emplace_back(std::make_shared<BasicInstructionRead>(translationTable[var->getName()], hardware));
          }
          else {
            const auto& arr = std::dynamic_pointer_cast<AstArray>(leftValue);
            const auto& arrIndex = arr->getArgument();
            if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
              const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
              const auto& temp = hardware->getTempRegister();
              basicInstructions.emplace_back(std::make_shared<BasicInstructionRead>(temp, hardware));
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToKnownArr>(
                  temp, translationTable[arr->getName()], arrIndexNum->getValue(), hardware));
            }
            else {
              const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
              const auto& temp = hardware->getTempRegister();
              basicInstructions.emplace_back(std::make_shared<BasicInstructionRead>(temp, hardware));
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToArr>(
                  temp, translationTable[arr->getName()], translationTable[arrIndexVar->getName()], hardware));
            }
          }
          break;
        }
        case AstCommandType::WRITE: {
          const auto& write = std::dynamic_pointer_cast<AstWrite>(command);
          const auto& value = write->getValue();
          if (value->getNodeType() == AstNodeType::NUMBER) {
            const auto& num = std::dynamic_pointer_cast<AstNumber>(value);
            const auto& temp = hardware->getTempRegister();
            basicInstructions.emplace_back(std::make_shared<BasicInstructionMovNumToVar>(num->getValue(), temp, hardware));
            basicInstructions.emplace_back(std::make_shared<BasicInstructionWrite>(temp, hardware));
          }
          else if (value->getNodeType() == AstNodeType::VARIABLE) {
            const auto& var = std::dynamic_pointer_cast<AstVariable>(value);
            basicInstructions.emplace_back(std::make_shared<BasicInstructionWrite>(translationTable[var->getName()], hardware));
          }
          else {
            const auto& arr = std::dynamic_pointer_cast<AstArray>(value);
            const auto& arrIndex = arr->getArgument();
            if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
              const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
              const auto& temp = hardware->getTempRegister();
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMovKnownArrToVar>(
                  translationTable[arr->getName()], arrIndexNum->getValue(), temp, hardware));
              basicInstructions.emplace_back(std::make_shared<BasicInstructionWrite>(temp, hardware));
            }
            else {
              const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
              const auto& temp = hardware->getTempRegister();
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMovArrToVar>(
                  translationTable[arr->getName()], translationTable[arrIndexVar->getName()], temp, hardware));
              basicInstructions.emplace_back(std::make_shared<BasicInstructionWrite>(temp, hardware));
            }
          }
          break;
        }
        case AstCommandType::ASSIGNMENT: {
          const auto& assignment = std::dynamic_pointer_cast<AstAssignment>(command);
          const auto& leftValue = assignment->getLvalue();
          const auto& expression = assignment->getExpression();
          if (expression->getRight() == nullptr) {
            const auto& newValue = expression->getLeft();
            if (newValue->getNodeType() == AstNodeType::NUMBER) {
              const auto& newNum = std::dynamic_pointer_cast<AstNumber>(newValue);
              if (leftValue->getNodeType() == AstNodeType::VARIABLE) {
                const auto& var = std::dynamic_pointer_cast<AstVariable>(leftValue);
                basicInstructions.emplace_back(std::make_shared<BasicInstructionMovNumToVar>(
                    newNum->getValue(), translationTable[var->getName()], hardware));
              }
              else {
                const auto& temp = hardware->getTempRegister();
                basicInstructions.emplace_back(std::make_shared<BasicInstructionMovNumToVar>(newNum->getValue(), temp, hardware));

                const auto& arr = std::dynamic_pointer_cast<AstArray>(leftValue);
                const auto& arrIndex = arr->getArgument();
                if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
                  const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
                  basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToKnownArr>(
                      temp, translationTable[arr->getName()], arrIndexNum->getValue(), hardware));
                }
                else {
                  const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
                  basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToArr>(
                      temp, translationTable[arr->getName()], translationTable[arrIndexVar->getName()], hardware));
                }
              }
            }
            else {
              const auto& newVar = std::dynamic_pointer_cast<AstVariable>(newValue);
              if (leftValue->getNodeType() == AstNodeType::VARIABLE) {
                const auto& var = std::dynamic_pointer_cast<AstVariable>(leftValue);
                basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToVar>(
                    translationTable[newVar->getName()], translationTable[var->getName()], hardware));
              }
              else {
                const auto& arr = std::dynamic_pointer_cast<AstArray>(leftValue);
                const auto& arrIndex = arr->getArgument();
                if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
                  const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
                  basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToKnownArr>(
                      translationTable[newVar->getName()], translationTable[arr->getName()], arrIndexNum->getValue(), hardware));
                }
                else {
                  const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
                  basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToArr>(
                      translationTable[newVar->getName()], translationTable[arr->getName()],
                      translationTable[arrIndexVar->getName()], hardware));
                }
              }
            }
          }
          else {
            const auto expressionType = convertMathOperationType(expression->getExpressionType());
            const auto& leftOperand = expression->getLeft();
            const auto& rightOperand = expression->getRight();
            const auto& result = leftValue->getNodeType() == AstNodeType::VARIABLE ? translationTable[leftValue->getName()]
                                                                                   : hardware->getTempRegister();

            if (leftOperand->getNodeType() == AstNodeType::NUMBER && rightOperand->getNodeType() == AstNodeType::NUMBER) {
              const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(leftOperand);
              const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(rightOperand);
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMathOperationNumNum>(
                  expressionType, leftNum->getValue(), rightNum->getValue(), result, hardware));
            }
            else if (leftOperand->getNodeType() == AstNodeType::NUMBER) {
              const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(leftOperand);
              const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(rightOperand);
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMathOperationNumVar>(
                  expressionType, leftNum->getValue(), translationTable[rightVar->getName()], result, hardware));
            }
            else if (rightOperand->getNodeType() == AstNodeType::NUMBER) {
              const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(leftOperand);
              const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(rightOperand);
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMathOperationVarNum>(
                  expressionType, translationTable[leftVar->getName()], rightNum->getValue(), result, hardware));
            }
            else {
              const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(leftOperand);
              const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(rightOperand);
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMathOperationVarVar>(
                  expressionType, translationTable[leftVar->getName()], translationTable[rightVar->getName()], result, hardware));
            }

            if (leftValue->getNodeType() == AstNodeType::ARRAY) {
              basicInstructions.emplace_back(
                  std::make_shared<BasicInstructionMovVarToVar>(result, translationTable[leftValue->getName()], hardware));
            }
          }
          break;
        }
        case AstCommandType::IF: {
          const auto& ifCommand = std::dynamic_pointer_cast<AstIf>(command);
          const auto& condition = ifCommand->getCondition();
          const auto& commands = ifCommand->getCommands();
          const auto& elseCommands = ifCommand->getElseCommands();

          const auto elseLabel = hardware->getLabel(scopes.top(), "else#", true);
          const auto endifLabel = hardware->getLabel(scopes.top(), "endif#", false);

          convertCondition(condition, elseLabel);
          convertToBasicInstructions(commands);
          if (elseCommands->getCommands().size() > 0) {
            basicInstructions.emplace_back(std::make_shared<BasicInstructionJump>(endifLabel, hardware));
          }
          basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(elseLabel, hardware));
          convertToBasicInstructions(elseCommands);
          if (elseCommands->getCommands().size() > 0) {
            basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(endifLabel, hardware));
          }
          break;
        }
        case AstCommandType::WHILE: {
          const auto whileCommand = std::dynamic_pointer_cast<AstWhile>(command);
          const auto& condition = whileCommand->getCondition();
          const auto& commands = whileCommand->getCommands();

          const auto whileLabel = hardware->getLabel(scopes.top(), "while#", true);
          const auto endwhileLabel = hardware->getLabel(scopes.top(), "endwhile#", false);

          basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(whileLabel, hardware));
          convertCondition(condition, endwhileLabel);
          convertToBasicInstructions(commands);
          basicInstructions.emplace_back(std::make_shared<BasicInstructionJump>(whileLabel, hardware));
          basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(endwhileLabel, hardware));
          break;
        }
        case AstCommandType::REPEAT: {
          const auto repeatCommand = std::dynamic_pointer_cast<AstRepeat>(command);
          const auto& condition = repeatCommand->getCondition();
          const auto& commands = repeatCommand->getCommands();

          const auto repeatLabel = hardware->getLabel(scopes.top(), "repeat#", true);

          basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(repeatLabel, hardware));
          convertToBasicInstructions(commands);
          convertCondition(condition, repeatLabel);
          break;
        }
        case AstCommandType::PROCEDURE_CALL: {
          const auto& procedureCall = std::dynamic_pointer_cast<AstProcedureCall>(command);
          const auto& procedureName = procedureCall->getName();
          const auto& args = procedureCall->getArgsList()->getArgs();
          auto argNames = std::vector<std::string>();
          for (auto arg : args) {
            const auto& var = std::dynamic_pointer_cast<AstVariable>(arg);
            argNames.emplace_back(translationTable[var->getName()]);
          }

          basicInstructions.emplace_back(std::make_shared<BasicInstructionPush>(hardware->getStackOffset(), argNames, hardware));
          hardware->incrementStackOffset(args.size());
          const auto& beginLabel = hardware->getGlobalLabel(procedureName, "begin#");
          basicInstructions.emplace_back(std::make_shared<BasicInstructionJump>(beginLabel, hardware));
          break;
        }
        case AstCommandType::UNDEFINED: {
          std::cout << "Unknown command type" << std::endl;
          break;
        }
      }
      break;
    }
    default:
      std::cout << "Unknown node type" << std::endl;
      ast->print(std::cout, 0);
      break;
  }
}

void Compiler::expandBasicInstructions() {
  hardware->initializeStack();
  for (auto instruction : basicInstructions) {
    const auto& expandedInstructions = instruction->expandToHardwareInstructions();
    machineCode.insert(machineCode.end(), expandedInstructions.begin(), expandedInstructions.end());
  }
}

void Compiler::assignRegistersAndMemory() {}

void Compiler::optimizeBasicInstructions() {}

void Compiler::optimizeMachineCode() {}

void Compiler::optimizeFinalMachineCode() {}

void Compiler::assignLabels() {}

BasicInstructionMathOperationType Compiler::convertMathOperationType(AstExpression::ExpressionType type) {
  switch (type) {
    case AstExpression::ExpressionType::ADD:
      return BasicInstructionMathOperationType::ADD;
    case AstExpression::ExpressionType::SUB:
      return BasicInstructionMathOperationType::SUB;
    case AstExpression::ExpressionType::MUL:
      return BasicInstructionMathOperationType::MUL;
    case AstExpression::ExpressionType::DIV:
      return BasicInstructionMathOperationType::DIV;
    case AstExpression::ExpressionType::MOD:
      return BasicInstructionMathOperationType::MOD;
    default:
      return BasicInstructionMathOperationType::UNDEFINED;
  }
}

std::shared_ptr<BasicInstruction> Compiler::castCondition(const std::shared_ptr<AstValue> left,
                                                          const std::shared_ptr<AstValue> right,
                                                          BasicInstructionConditionType jumpCondition, std::string label) {
  if (left->getNodeType() == AstNodeType::NUMBER && right->getNodeType() == AstNodeType::NUMBER) {
    const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(left);
    const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(right);

    return std::make_shared<BasicInstructionConditionJumpNumNum>(leftNum->getValue(), rightNum->getValue(), jumpCondition, label,
                                                                 hardware);
  }
  else if (left->getNodeType() == AstNodeType::NUMBER) {
    const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(left);
    const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(right);

    return std::make_shared<BasicInstructionConditionJumpNumVar>(leftNum->getValue(), translationTable[rightVar->getName()],
                                                                 jumpCondition, label, hardware);
  }
  else if (right->getNodeType() == AstNodeType::NUMBER) {
    const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(left);
    const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(right);

    return std::make_shared<BasicInstructionConditionJumpVarNum>(translationTable[leftVar->getName()], rightNum->getValue(),
                                                                 jumpCondition, label, hardware);
  }
  else {
    const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(left);
    const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(right);

    return std::make_shared<BasicInstructionConditionJumpVarVar>(
        translationTable[leftVar->getName()], translationTable[rightVar->getName()], jumpCondition, label, hardware);
  }
}

void Compiler::convertCondition(const std::shared_ptr<AstCondition> condition, std::string label) {
  switch (condition->getConditionType()) {
    case AstConditionType::GREATER: {
      basicInstructions.emplace_back(
          castCondition(condition->getLeft(), condition->getRight(), BasicInstructionConditionType::LESS_EQUAL, label));
      break;
    }
    case AstConditionType::LESS: {
      basicInstructions.emplace_back(
          castCondition(condition->getRight(), condition->getLeft(), BasicInstructionConditionType::LESS_EQUAL, label));
      break;
    }
    case AstConditionType::GREATER_EQUAL: {
      basicInstructions.emplace_back(
          castCondition(condition->getRight(), condition->getLeft(), BasicInstructionConditionType::LESS, label));
      break;
    }
    case AstConditionType::LESS_EQUAL: {
      basicInstructions.emplace_back(
          castCondition(condition->getRight(), condition->getLeft(), BasicInstructionConditionType::LESS, label));
      break;
    }
    case AstConditionType::EQUAL: {
      basicInstructions.emplace_back(
          castCondition(condition->getLeft(), condition->getRight(), BasicInstructionConditionType::NOT_EQUAL, label));
      break;
    }
    case AstConditionType::NOT_EQUAL: {
      basicInstructions.emplace_back(
          castCondition(condition->getLeft(), condition->getRight(), BasicInstructionConditionType::EQUAL, label));
      break;
    }
  }
}
