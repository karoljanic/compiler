#include "../../include/ast/AstWhile.hpp"

AstWhile::AstWhile() : AstCommand(CommandType::WHILE) {}

AstWhile::AstWhile(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands)
	: AstCommand(CommandType::WHILE), condition(condition), commands(commands) {}

const std::shared_ptr<AstCondition> &AstWhile::getCondition() const {
  return condition;
}

const std::shared_ptr<AstCommands> &AstWhile::getCommands() const {
  return commands;
}

void AstWhile::setCondition(const std::shared_ptr<AstCondition> &condition) {
  AstWhile::condition = condition;
}

void AstWhile::setCommands(const std::shared_ptr<AstCommands> &commands) {
  AstWhile::commands = commands;
}

std::shared_ptr<AstNode> AstWhile::copy(const std::map<std::string, std::string> &rewriteTable) const {
  return std::make_shared<AstWhile>(std::dynamic_pointer_cast<AstCondition>(condition->copy(rewriteTable)),
									std::dynamic_pointer_cast<AstCommands>(commands->copy(rewriteTable)));
}

void AstWhile::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "WHILE ";
  condition->print(out, 0);
  out << " DO" << std::endl;
  commands->print(out, tab + 4);
  out << std::string(tab, ' ') << "ENDWHILE" << std::endl;
}
