#include "../../include/ast/AstCommand.hpp"

AstCommand::AstCommand() : AstNode(NodeType::COMMAND), commandType(CommandType::UNDEFINED) {}

AstCommand::AstCommand(CommandType type) : AstNode(NodeType::COMMAND), commandType(type) {}

AstCommand::CommandType AstCommand::getCommandType() const {
  return commandType;
}

void AstCommand::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "command";
}
