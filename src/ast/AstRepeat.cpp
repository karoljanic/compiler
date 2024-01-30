#include "../../include/ast/AstRepeat.hpp"

AstRepeat::AstRepeat() : AstCommand(CommandType::REPEAT) {}

AstRepeat::AstRepeat(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands)
	: AstCommand(CommandType::REPEAT), condition(std::move(condition)), commands(std::move(commands)) {}

const std::shared_ptr<AstCondition> &AstRepeat::getCondition() const {
  return condition;
}

const std::shared_ptr<AstCommands> &AstRepeat::getCommands() const {
  return commands;
}

void AstRepeat::setCondition(const std::shared_ptr<AstCondition> &condition) {
  AstRepeat::condition = condition;
}

void AstRepeat::setCommands(const std::shared_ptr<AstCommands> &commands) {
  AstRepeat::commands = commands;
}

std::shared_ptr<AstNode> AstRepeat::copy(const std::map<std::string, std::string> &rewriteTable) const {
  return std::make_shared<AstRepeat>(std::dynamic_pointer_cast<AstCondition>(condition->copy(rewriteTable)),
									 std::dynamic_pointer_cast<AstCommands>(commands->copy(rewriteTable)));
}

void AstRepeat::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "REPEAT" << std::endl;
  commands->print(out, tab + 4);
  out << std::string(tab, ' ') << "UNTIL ";
  condition->print(out, tab + 4);
}
