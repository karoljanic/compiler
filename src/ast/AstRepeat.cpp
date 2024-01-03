#include "../../include/ast/AstRepeat.hpp"

AstRepeat::AstRepeat() : AstCommand(CommandType::REPEAT) {}

AstRepeat::AstRepeat(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands)
    : AstCommand(CommandType::REPEAT), condition(std::move(condition)), commands(std::move(commands)) {}

const std::shared_ptr<AstCondition>& AstRepeat::getCondition() const {
  return condition;
}

const std::shared_ptr<AstCommands>& AstRepeat::getCommands() const {
  return commands;
}

void AstRepeat::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "REPEAT" << std::endl;
  commands->print(out, tab + 4);
  out << std::string(tab, ' ') << "UNTIL ";
  condition->print(out, tab + 4);
}
