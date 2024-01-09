#include "../../include/ast/AstIf.hpp"

AstIf::AstIf() : AstCommand(AstCommand::IF) {}

AstIf::AstIf(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands,
             std::shared_ptr<AstCommands> elseCommands)
    : AstCommand(AstCommand::IF), condition(condition), commands(commands), elseCommands(elseCommands) {}

const std::shared_ptr<AstCondition>& AstIf::getCondition() const {
  return condition;
}

const std::shared_ptr<AstCommands>& AstIf::getCommands() const {
  return commands;
}

const std::shared_ptr<AstCommands>& AstIf::getElseCommands() const {
  return elseCommands;
}

void AstIf::print(std::ostream& out, int tab) const {
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
