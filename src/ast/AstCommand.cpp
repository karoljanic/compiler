#include <iostream>

#include "../../include/ast/AstCommand.hpp"

AstCommand::AstCommand() : AstNode(NodeType::COMMAND), commandType(CommandType::DEFAULT) {}

AstCommand::AstCommand(CommandType type) : AstNode(NodeType::COMMAND), commandType(type) {}

AstCommand::CommandType AstCommand::getCommandType() const {
  return commandType;
}

void AstCommand::print() const {
  std::cout << "Command: ";
  switch (commandType) {
    case CommandType::ASSIGNMENT:
      std::cout << "Assignment";
      break;
    case CommandType::IF:
      std::cout << "If";
      break;
    case CommandType::WHILE:
      std::cout << "While";
      break;
    case CommandType::REPEAT:
      std::cout << "Repeat";
      break;
    case CommandType::PROCEDURE_CALL:
      std::cout << "Procedure call";
      break;
    case CommandType::READ:
      std::cout << "Read";
      break;
    case CommandType::WRITE:
      std::cout << "Write";
      break;
    case CommandType::DEFAULT:
    default:
      std::cout << "Not implemented";
      break;
  }
  std::cout << std::endl;
}
