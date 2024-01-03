#include <iostream>

#include "../../include/ast/AstLeftValue.hpp"

AstLeftValue::AstLeftValue() : AstValue(NodeType::LVALUE) {}

AstLeftValue::AstLeftValue(NodeType type) : AstValue(type) {}

void AstLeftValue::print() const {
  std::cout << "AstLeftValue" << std::endl;
}
