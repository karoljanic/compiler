#include "../../include/ast/AstCommand.hpp"

AstCommand::AstCommand() : AstNode(NodeType::COMMAND), commandType(CommandType::UNDEFINED) {}

AstCommand::AstCommand(CommandType type) : AstNode(NodeType::COMMAND), commandType(type) {}

AstCommand::CommandType AstCommand::getCommandType() const {
  return commandType;
}

std::shared_ptr<AstNode> AstCommand::copy(const std::map<std::string, std::string> & /*&rewriteTable*/) const {
  return std::make_shared<AstCommand>(commandType);
}

void AstCommand::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "command";
}
