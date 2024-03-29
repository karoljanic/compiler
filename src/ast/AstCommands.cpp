#include "../../include/ast/AstCommands.hpp"

AstCommands::AstCommands() : AstNode(NodeType::COMMANDS) {}

AstCommands::AstCommands(std::shared_ptr<AstCommand> command) : AstNode(NodeType::COMMANDS) {
  commands.push_back(command);
}

void AstCommands::addCommand(std::shared_ptr<AstCommand> command) {
  commands.push_back(command);
}

const std::vector<std::shared_ptr<AstCommand>> &AstCommands::getCommands() {
  return commands;
}

std::shared_ptr<AstNode> AstCommands::copy(const std::map<std::string, std::string> &rewriteTable) const {
  auto result = std::make_shared<AstCommands>();
  for (auto &command : commands) {
	result->addCommand(std::dynamic_pointer_cast<AstCommand>(command->copy(rewriteTable)));
  }
  return result;
}

void AstCommands::print(std::ostream &out, int tab) const {
  for (auto &command : commands) {
	command->print(out, tab);
	out << std::endl;
  }
}
