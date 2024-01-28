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
#include "../bblocks/BasicInstructionCondJumpNN.hpp"
#include "../bblocks/BasicInstructionCondJumpNV.hpp"
#include "../bblocks/BasicInstructionCondJumpVN.hpp"
#include "../bblocks/BasicInstructionCondJumpVV.hpp"
#include "../bblocks/BasicInstructionHalt.hpp"
#include "../bblocks/BasicInstructionJump.hpp"
#include "../bblocks/BasicInstructionJumpFun.hpp"
#include "../bblocks/BasicInstructionJumpRelative.hpp"
#include "../bblocks/BasicInstructionLabel.hpp"
#include "../bblocks/BasicInstructionMathNN.hpp"
#include "../bblocks/BasicInstructionMathNV.hpp"
#include "../bblocks/BasicInstructionMathVN.hpp"
#include "../bblocks/BasicInstructionMathVV.hpp"
#include "../bblocks/BasicInstructionMovAV.hpp"
#include "../bblocks/BasicInstructionMovKAV.hpp"
#include "../bblocks/BasicInstructionMovNV.hpp"
#include "../bblocks/BasicInstructionMovVA.hpp"
#include "../bblocks/BasicInstructionMovVKA.hpp"
#include "../bblocks/BasicInstructionMovVV.hpp"
#include "../bblocks/BasicInstructionPop.hpp"
#include "../bblocks/BasicInstructionPush.hpp"
#include "../bblocks/BasicInstructionRead.hpp"
#include "../bblocks/BasicInstructionWrite.hpp"
#include "../bblocks/BasicInstructionRestoreState.hpp"
#include "../bblocks/BasicInstructionSaveState.hpp"

#include "../core/ControlFlowGraph.hpp"
#include "../core/Hardware.hpp"
#include "../core/RegistersLinearScan.hpp"

typedef AstNode::NodeType AstNodeType;
typedef AstExpression::ExpressionType AstExpressionType;
typedef AstCommand::CommandType AstCommandType;
typedef AstCondition::ConditionType AstConditionType;

class Compiler {
 private:
  std::shared_ptr<AstNode> ast;
  ControlFlowGraph firstControlFlowGraph;
  ControlFlowGraph controlFlowGraph;
  std::vector<ControlFlowGraphNode> machineCodeWithVariablesAndLabels;
  std::vector<MachineCodeType> machineCodeWithLabels;
  std::vector<std::pair<HardwareInstruction, std::string>> machineCode;
  std::shared_ptr<Hardware> hardware;
  RegistersLinearScan registersLinearScan;

  std::stack<std::string> scopes;
  std::map<std::string, std::string> translationTable;
  std::vector<uint64_t> parentsIds;
  std::map<uint64_t, std::vector<std::string>> currLabels;
  std::vector<std::string> currProcedureArgs;
  std::map<std::string, std::vector<std::string>> proceduresArgs;
  std::vector<std::string> usedProcedures;
  std::map<std::string, std::vector<std::string>> procedureCallsInProcedure;

 public:
  Compiler();
  Compiler(std::shared_ptr<AstNode> ast);

  void generateMachineCode(std::ofstream &outputFile);
  void generateMachineCodeWithDebug(std::ofstream &astFile,
									std::ofstream &basicBlocksFile,
									std::ofstream &machineCodeFile,
									std::ofstream &outputFile);

 private:
  void pasteProcedures();
  void pasteProceduresUtil(const std::shared_ptr<AstDeclarations> &declarations,
						   const std::shared_ptr<AstCommands> &commands,
						   const std::shared_ptr<AstProcedures> &procedures,
						   std::shared_ptr<AstDeclarations> &newDeclarations,
						   std::shared_ptr<AstCommands> &newCommands);
  void convertToFirstControlFlowGraph();
  void convertToControlFlowGraph();
  void optimizeControlFlowGraph();
  void expandAndOptimizeBasicInstructions();
  void findRegisters();
  void optimizeMachineCode();
  void findLabels();
  void generateOutput(std::ofstream &outputFile);

  void parseAstMain(std::shared_ptr<AstMain> node,
					std::pair<std::string, std::vector<ControlFlowGraphNode>> &instructions);
  void parseAstProcedure(std::shared_ptr<AstProcedure> node,
						 std::pair<std::string, std::vector<ControlFlowGraphNode>> &instructions);
  void parseAstDeclarations(std::shared_ptr<AstDeclarations> node);
  void parseAstProcedureHeader(std::shared_ptr<AstProcedureHeader>);
  void parseAstCommand(std::shared_ptr<AstCommand> node, std::vector<ControlFlowGraphNode> &instructions);
  std::vector<ControlFlowGraphNode> parseAstCondition(std::shared_ptr<AstCondition> node, const std::string &jumpLabel);
  ControlFlowGraphNode parseMoveLvalueToVariable(std::shared_ptr<AstLeftValue> lvalue, std::string variableName);
  bool isProcedureArgument(const std::string &name);
  bool pasteProcedureEfficiency(const std::string &name);
};

#endif  // COMPILER_HPP
