#include <iostream>

#include "../../include/ast/AstVariable.hpp"

AstVariable::AstVariable() : AstLeftValue(NodeType::VARIABLE) {}

AstVariable::AstVariable(const std::string name) : AstLeftValue(NodeType::VARIABLE), name(name) {}

void AstVariable::print() const {
  std::cout << name;
}
