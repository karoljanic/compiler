#include <iostream>

#include "../../include/ast/AstIf.hpp"

AstIf::AstIf() : AstCommand(AstCommand::IF) {}

AstIf::AstIf(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands,
             std::shared_ptr<AstCommands> elseCommands)
    : AstCommand(AstCommand::IF), condition(condition), commands(commands), elseCommands(elseCommands) {}

std::shared_ptr<AstCondition> AstIf::getCondition() const {
  return condition;
}

std::shared_ptr<AstCommands> AstIf::getCommands() const {
  return commands;
}

std::shared_ptr<AstCommands> AstIf::getElseCommands() const {
  return elseCommands;
}

void AstIf::print() const {
  std::cout << "AstIf {" << std::endl;
  std::cout << "condition: ";
  condition->print();
  std::cout << std::endl;
  std::cout << "commands: ";
  commands->print();
  std::cout << std::endl;
  std::cout << "elseCommands: ";
  elseCommands->print();
  std::cout << std::endl;
  std::cout << "}" << std::endl;
}
