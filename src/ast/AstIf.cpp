#include "../../include/ast/AstIf.hpp"

AstIf::AstIf() : AstCommand(AstCommand::IF) {}

AstIf::AstIf(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands,
			 std::shared_ptr<AstCommands> elseCommands)
	: AstCommand(AstCommand::IF), condition(condition), commands(commands), elseCommands(elseCommands) {}

const std::shared_ptr<AstCondition> &AstIf::getCondition() const {
  return condition;
}

const std::shared_ptr<AstCommands> &AstIf::getCommands() const {
  return commands;
}

const std::shared_ptr<AstCommands> &AstIf::getElseCommands() const {
  return elseCommands;
}

std::shared_ptr<AstNode> AstIf::copy(const std::map<std::string, std::string> &rewriteTable) const {
  if (elseCommands == nullptr) {
	return std::make_shared<AstIf>(std::dynamic_pointer_cast<AstCondition>(condition->copy(rewriteTable)),
								   std::dynamic_pointer_cast<AstCommands>(commands->copy(rewriteTable)),
								   nullptr);
  }
  return std::make_shared<AstIf>(std::dynamic_pointer_cast<AstCondition>(condition->copy(rewriteTable)),
								 std::dynamic_pointer_cast<AstCommands>(commands->copy(rewriteTable)),
								 std::dynamic_pointer_cast<AstCommands>(elseCommands->copy(rewriteTable)));
}

void AstIf::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "IF ";
  condition->print(out, 0);
  out << std::endl;
  out << std::string(tab, ' ') << "THEN" << std::endl;
  commands->print(out, tab + 4);
  if (elseCommands != nullptr && elseCommands->getCommands().size() > 0) {
	out << std::string(tab, ' ') << "ELSE" << std::endl;
	elseCommands->print(out, tab + 4);
  }
  out << std::string(tab, ' ') << "ENDIF" << std::endl;
}
