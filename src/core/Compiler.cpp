#include <iostream>

#include "../../include/core/Compiler.hpp"

Compiler::Compiler() {}

Compiler::Compiler(std::shared_ptr<AstNode> ast) : ast(ast) {}

void Compiler::convertToBasicInstructions() {
  convertToBasicInstructionsUtil(ast);
}

void Compiler::generateMachineCode(std::ofstream& /*outputFile*/) {
  std::cout << std::endl << "Generating machine code..." << std::endl;
  for (auto instruction : basicInstructions) {
    instruction->print(std::cout);
    std::cout << std::endl;
  }
}

std::string Compiler::getTempVariable() {
  tempsCounter++;
  return "t" + std::to_string(tempsCounter);
}

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

    return std::make_shared<BasicInstructionConditionJumpNumNum>(leftNum->getValue(), rightNum->getValue(), jumpCondition, label);
  }
  else if (left->getNodeType() == AstNodeType::NUMBER) {
    const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(left);
    const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(right);

    return std::make_shared<BasicInstructionConditionJumpNumVar>(leftNum->getValue(), translationTable[rightVar->getName()],
                                                                 jumpCondition, label);
  }
  else if (right->getNodeType() == AstNodeType::NUMBER) {
    const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(left);
    const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(right);

    return std::make_shared<BasicInstructionConditionJumpVarNum>(translationTable[leftVar->getName()], rightNum->getValue(),
                                                                 jumpCondition, label);
  }
  else {
    const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(left);
    const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(right);

    return std::make_shared<BasicInstructionConditionJumpVarVar>(translationTable[leftVar->getName()],
                                                                 translationTable[rightVar->getName()], jumpCondition, label);
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

void Compiler::convertToBasicInstructionsUtil(const std::shared_ptr<AstNode>& ast) {
  if (ast == nullptr) {
    return;
  }

  switch (ast->getNodeType()) {
    case AstNodeType::PROGRAM: {
      const auto& program = std::dynamic_pointer_cast<AstProgram>(ast);
      convertToBasicInstructionsUtil(program->getMain());
      convertToBasicInstructionsUtil(program->getProcedures());
      break;
    }
    case AstNodeType::PROCEDURES: {
      const auto& procedures = std::dynamic_pointer_cast<AstProcedures>(ast);
      for (auto procedure : procedures->getProcedures()) {
        convertToBasicInstructionsUtil(procedure);
      }
      break;
    }
    case AstNodeType::PROCEDURE: {
      const auto& procedure = std::dynamic_pointer_cast<AstProcedure>(ast);
      const auto& procedureName = procedure->getHeader()->getName();
      scopes.push(procedureName);
      basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(procedureName + "@begin"));
      convertToBasicInstructionsUtil(procedure->getHeader());
      convertToBasicInstructionsUtil(procedure->getDeclarations());
      std::vector<std::string> args;
      for (auto arg : procedure->getHeader()->getArgsDeclaration()->getArgs()) {
        args.emplace_back(translationTable[arg->getName()]);
      }
      std::reverse(args.begin(), args.end());
      basicInstructions.emplace_back(std::make_shared<BasicInstructionPop>(stackOffset, args));
      convertToBasicInstructionsUtil(procedure->getCommands());
      basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(procedureName + "@end"));
      scopes.pop();
      break;
    }
    case AstNodeType::MAIN: {
      const auto& main = std::dynamic_pointer_cast<AstMain>(ast);
      scopes.push("@main");
      basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>("@main@begin"));
      convertToBasicInstructionsUtil(main->getDeclarations());
      convertToBasicInstructionsUtil(main->getCommands());
      basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>("@main@end"));
      scopes.pop();
      break;
    }
    case AstNodeType::PROCEDURE_HEADER: {
      const auto& procedureHeader = std::dynamic_pointer_cast<AstProcedureHeader>(ast);
      const auto& args = procedureHeader->getArgsDeclaration()->getArgs();
      for (auto arg : args) {
        const auto& name = arg->getName();
        translationTable[name] = scopes.top() + "@" + name;
      }
      break;
    }
    case AstNodeType::DECLARATIONS: {
      const auto& declarations = std::dynamic_pointer_cast<AstDeclarations>(ast);
      for (auto declaration : declarations->getDeclarations()) {
        const auto& name = declaration->getName();
        translationTable[name] = scopes.top() + "@" + name;
        if (declaration->getNodeType() == AstNodeType::ARRAY) {
          const auto& arr = std::dynamic_pointer_cast<AstArray>(declaration);
          const auto& index = std::dynamic_pointer_cast<AstNumber>(arr->getArgument());
          hardware.allocateArray(name, index->getValue());
        }
      }
      break;
    }
    case AstNodeType::COMMANDS: {
      const auto& commands = std::dynamic_pointer_cast<AstCommands>(ast);
      for (auto command : commands->getCommands()) {
        convertToBasicInstructionsUtil(command);
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
            basicInstructions.emplace_back(std::make_shared<BasicInstructionRead>(translationTable[var->getName()]));
          }
          else {
            const auto& arr = std::dynamic_pointer_cast<AstArray>(leftValue);
            const auto& arrIndex = arr->getArgument();
            if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
              const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
              const auto& temp = getTempVariable();
              basicInstructions.emplace_back(std::make_shared<BasicInstructionRead>(temp));
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToKnownArr>(
                  temp, translationTable[arr->getName()], arrIndexNum->getValue()));
            }
            else {
              const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
              const auto& temp = getTempVariable();
              basicInstructions.emplace_back(std::make_shared<BasicInstructionRead>(temp));
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToArr>(
                  temp, translationTable[arr->getName()], translationTable[arrIndexVar->getName()]));
            }
          }
          break;
        }
        case AstCommandType::WRITE: {
          const auto& write = std::dynamic_pointer_cast<AstWrite>(command);
          const auto& value = write->getValue();
          if (value->getNodeType() == AstNodeType::NUMBER) {
            const auto& num = std::dynamic_pointer_cast<AstNumber>(value);
            const auto& temp = getTempVariable();
            basicInstructions.emplace_back(std::make_shared<BasicInstructionMovNumToVar>(num->getValue(), temp));
            basicInstructions.emplace_back(std::make_shared<BasicInstructionWrite>(temp));
          }
          else if (value->getNodeType() == AstNodeType::VARIABLE) {
            const auto& var = std::dynamic_pointer_cast<AstVariable>(value);
            basicInstructions.emplace_back(std::make_shared<BasicInstructionWrite>(translationTable[var->getName()]));
          }
          else {
            const auto& arr = std::dynamic_pointer_cast<AstArray>(value);
            const auto& arrIndex = arr->getArgument();
            if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
              const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
              const auto& temp = getTempVariable();
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMovKnownArrToVar>(translationTable[arr->getName()],
                                                                                                arrIndexNum->getValue(), temp));
              basicInstructions.emplace_back(std::make_shared<BasicInstructionWrite>(temp));
            }
            else {
              const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
              const auto& temp = getTempVariable();
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMovArrToVar>(
                  translationTable[arr->getName()], translationTable[arrIndexVar->getName()], temp));
              basicInstructions.emplace_back(std::make_shared<BasicInstructionWrite>(temp));
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
                basicInstructions.emplace_back(
                    std::make_shared<BasicInstructionMovNumToVar>(newNum->getValue(), translationTable[var->getName()]));
              }
              else {
                const auto& temp = getTempVariable();
                basicInstructions.emplace_back(std::make_shared<BasicInstructionMovNumToVar>(newNum->getValue(), temp));

                const auto& arr = std::dynamic_pointer_cast<AstArray>(leftValue);
                const auto& arrIndex = arr->getArgument();
                if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
                  const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
                  basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToKnownArr>(
                      temp, translationTable[arr->getName()], arrIndexNum->getValue()));
                }
                else {
                  const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
                  basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToArr>(
                      temp, translationTable[arr->getName()], translationTable[arrIndexVar->getName()]));
                }
              }
            }
            else {
              const auto& newVar = std::dynamic_pointer_cast<AstVariable>(newValue);
              if (leftValue->getNodeType() == AstNodeType::VARIABLE) {
                const auto& var = std::dynamic_pointer_cast<AstVariable>(leftValue);
                basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToVar>(translationTable[newVar->getName()],
                                                                                             translationTable[var->getName()]));
              }
              else {
                const auto& arr = std::dynamic_pointer_cast<AstArray>(leftValue);
                const auto& arrIndex = arr->getArgument();
                if (arrIndex->getNodeType() == AstNodeType::NUMBER) {
                  const auto arrIndexNum = std::dynamic_pointer_cast<AstNumber>(arrIndex);
                  basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToKnownArr>(
                      translationTable[newVar->getName()], translationTable[arr->getName()], arrIndexNum->getValue()));
                }
                else {
                  const auto& arrIndexVar = std::dynamic_pointer_cast<AstVariable>(arrIndex);
                  basicInstructions.emplace_back(std::make_shared<BasicInstructionMovVarToArr>(
                      translationTable[newVar->getName()], translationTable[arr->getName()],
                      translationTable[arrIndexVar->getName()]));
                }
              }
            }
          }
          else {
            const auto expressionType = convertMathOperationType(expression->getExpressionType());
            const auto& leftOperand = expression->getLeft();
            const auto& rightOperand = expression->getRight();
            const auto& result =
                leftValue->getNodeType() == AstNodeType::VARIABLE ? translationTable[leftValue->getName()] : getTempVariable();

            if (leftOperand->getNodeType() == AstNodeType::NUMBER && rightOperand->getNodeType() == AstNodeType::NUMBER) {
              const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(leftOperand);
              const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(rightOperand);
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMathOperationNumNum>(
                  expressionType, leftNum->getValue(), rightNum->getValue(), result));
            }
            else if (leftOperand->getNodeType() == AstNodeType::NUMBER) {
              const auto& leftNum = std::dynamic_pointer_cast<AstNumber>(leftOperand);
              const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(rightOperand);
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMathOperationNumVar>(
                  expressionType, leftNum->getValue(), translationTable[rightVar->getName()], result));
            }
            else if (rightOperand->getNodeType() == AstNodeType::NUMBER) {
              const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(leftOperand);
              const auto& rightNum = std::dynamic_pointer_cast<AstNumber>(rightOperand);
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMathOperationVarNum>(
                  expressionType, translationTable[leftVar->getName()], rightNum->getValue(), result));
            }
            else {
              const auto& leftVar = std::dynamic_pointer_cast<AstVariable>(leftOperand);
              const auto& rightVar = std::dynamic_pointer_cast<AstVariable>(rightOperand);
              basicInstructions.emplace_back(std::make_shared<BasicInstructionMathOperationVarVar>(
                  expressionType, translationTable[leftVar->getName()], translationTable[rightVar->getName()], result));
            }

            if (leftValue->getNodeType() == AstNodeType::ARRAY) {
              basicInstructions.emplace_back(
                  std::make_shared<BasicInstructionMovVarToVar>(result, translationTable[leftValue->getName()]));
            }
          }
          break;
        }
        case AstCommandType::IF: {
          const auto& ifCommand = std::dynamic_pointer_cast<AstIf>(command);
          const auto& condition = ifCommand->getCondition();
          const auto& commands = ifCommand->getCommands();
          const auto& elseCommands = ifCommand->getElseCommands();

          labelsCounter++;
          const auto elseLabel = scopes.top() + "@else" + std::to_string(labelsCounter);
          const auto endifLabel = scopes.top() + "@endif" + std::to_string(labelsCounter);

          convertCondition(condition, elseLabel);
          convertToBasicInstructionsUtil(commands);
          if (elseCommands->getCommands().size() > 0) {
            basicInstructions.emplace_back(std::make_shared<BasicInstructionJump>(endifLabel));
          }
          basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(elseLabel));
          convertToBasicInstructionsUtil(elseCommands);
          if (elseCommands->getCommands().size() > 0) {
            basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(endifLabel));
          }
          break;
        }
        case AstCommandType::WHILE: {
          const auto whileCommand = std::dynamic_pointer_cast<AstWhile>(command);
          const auto& condition = whileCommand->getCondition();
          const auto& commands = whileCommand->getCommands();

          labelsCounter++;
          const auto whileLabel = scopes.top() + "@while" + std::to_string(labelsCounter);
          const auto endwhileLabel = scopes.top() + "@endwhile" + std::to_string(labelsCounter);

          basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(whileLabel));
          convertCondition(condition, endwhileLabel);
          convertToBasicInstructionsUtil(commands);
          basicInstructions.emplace_back(std::make_shared<BasicInstructionJump>(whileLabel));
          basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(endwhileLabel));
          break;
        }
        case AstCommandType::REPEAT: {
          const auto repeatCommand = std::dynamic_pointer_cast<AstRepeat>(command);
          const auto& condition = repeatCommand->getCondition();
          const auto& commands = repeatCommand->getCommands();

          labelsCounter++;
          const auto repeatLabel = scopes.top() + "@repeat" + std::to_string(labelsCounter);

          basicInstructions.emplace_back(std::make_shared<BasicInstructionLabel>(repeatLabel));
          convertToBasicInstructionsUtil(commands);
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

          basicInstructions.emplace_back(std::make_shared<BasicInstructionPush>(stackOffset, argNames));
          stackOffset += args.size();
          basicInstructions.emplace_back(std::make_shared<BasicInstructionJump>(procedureName + "@begin"));
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