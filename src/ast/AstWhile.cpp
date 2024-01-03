#include "../../include/ast/AstWhile.hpp"

AstWhile::AstWhile() : AstCommand(CommandType::WHILE) {}

AstWhile::AstWhile(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands)
    : AstCommand(CommandType::WHILE), condition(condition), commands(commands) {}

const std::shared_ptr<AstCondition>& AstWhile::getCondition() const {
  return condition;
}

const std::shared_ptr<AstCommands>& AstWhile::getCommands() const {
  return commands;
}

void AstWhile::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "WHILE ";
  condition->print(out, 0);
  out << " DO" << std::endl;
  commands->print(out, tab + 4);
  out << std::string(tab, ' ') << "ENDWHILE" << std::endl;
}
