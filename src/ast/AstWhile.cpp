#include <iostream>

#include "../../include/ast/AstWhile.hpp"

AstWhile::AstWhile() : AstCommand(CommandType::WHILE) {}

AstWhile::AstWhile(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands)
    : AstCommand(CommandType::WHILE), condition(condition), commands(commands) {}

std::shared_ptr<AstCondition> AstWhile::getCondition() const {
  return condition;
}

std::shared_ptr<AstCommands> AstWhile::getCommands() const {
  return commands;
}

void AstWhile::print() const {
  std::cout << "while ";
  condition->print();
  std::cout << " do" << std::endl;
  commands->print();
  std::cout << "endwhile" << std::endl;
}
