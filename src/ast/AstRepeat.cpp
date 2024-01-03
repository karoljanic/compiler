#include <iostream>

#include "../../include/ast/AstRepeat.hpp"

AstRepeat::AstRepeat() : AstCommand(CommandType::REPEAT) {}

AstRepeat::AstRepeat(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands)
    : AstCommand(CommandType::REPEAT), condition(std::move(condition)), commands(std::move(commands)) {}

std::shared_ptr<AstCondition> AstRepeat::getCondition() const {
  return condition;
}

std::shared_ptr<AstCommands> AstRepeat::getCommands() const {
  return commands;
}

void AstRepeat::print() const {
  std::cout << "Repeat" << std::endl;
  condition->print();
  commands->print();
}
