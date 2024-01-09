#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <vector>

#include "../ast/AstArgsDeclaration.hpp"
#include "../ast/AstArgsList.hpp"
#include "../ast/AstArray.hpp"
#include "../ast/AstAssignment.hpp"
#include "../ast/AstCommand.hpp"
#include "../ast/AstCommands.hpp"
#include "../ast/AstCondition.hpp"
#include "../ast/AstDeclarations.hpp"
#include "../ast/AstExpression.hpp"
#include "../ast/AstIf.hpp"
#include "../ast/AstLeftValue.hpp"
#include "../ast/AstMain.hpp"
#include "../ast/AstNode.hpp"
#include "../ast/AstNumber.hpp"
#include "../ast/AstProcedure.hpp"
#include "../ast/AstProcedureCall.hpp"
#include "../ast/AstProcedureHeader.hpp"
#include "../ast/AstProcedures.hpp"
#include "../ast/AstProgram.hpp"
#include "../ast/AstRead.hpp"
#include "../ast/AstRepeat.hpp"
#include "../ast/AstRightValue.hpp"
#include "../ast/AstValue.hpp"
#include "../ast/AstVariable.hpp"
#include "../ast/AstWhile.hpp"
#include "../ast/AstWrite.hpp"

#include "../bblocks/BasicInstruction.hpp"
#include "../bblocks/BasicInstructionIO.hpp"
#include "../bblocks/BasicInstructionJump.hpp"
#include "../bblocks/BasicInstructionMath.hpp"
#include "../bblocks/BasicInstructionMov.hpp"
#include "../bblocks/BasicInstructionStack.hpp"

#include "../core/Hardware.hpp"

typedef AstNode::NodeType AstNodeType;
typedef AstCommand::CommandType AstCommandType;
typedef AstCondition::ConditionType AstConditionType;

class Compiler {
 private:
  std::shared_ptr<AstNode> ast;
  std::vector<std::shared_ptr<BasicInstruction>> basicInstructions;
  std::vector<std::pair<HardwareInstruction, std::string>> machineCode;
  std::shared_ptr<Hardware> hardware;

  std::stack<std::string> scopes;
  std::map<std::string, std::string> translationTable;

 public:
  Compiler();
  Compiler(std::shared_ptr<AstNode> ast);

  void generateMachineCode(std::ofstream& outputFile);
  void generateMachineCodeWithDebug(std::ofstream& basicInstructionsFile, std::ofstream& partialMachineCodeFile,
                                    std::ofstream& finalMachineCodeFile, std::ofstream& outputFile);

 private:
  void convertToBasicInstructions(const std::shared_ptr<AstNode>& ast);
  void expandBasicInstructions();
  void assignRegistersAndMemory();
  void optimizeBasicInstructions();
  void optimizeMachineCode();
  void optimizeFinalMachineCode();
  void assignLabels();

  BasicInstructionMathOperationType convertMathOperationType(AstExpression::ExpressionType type);
  std::shared_ptr<BasicInstruction> castCondition(const std::shared_ptr<AstValue> left, const std::shared_ptr<AstValue> right,
                                                  BasicInstructionConditionType jumpCondition, std::string label);
  void convertCondition(const std::shared_ptr<AstCondition> condition, std::string label);
};

#endif  // COMPILER_HPP
