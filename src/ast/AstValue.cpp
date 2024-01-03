#include <iostream>

#include "../../include/ast/AstValue.hpp"

AstValue::AstValue() : AstNode(NodeType::VALUE) {}

AstValue::AstValue(NodeType type) : AstNode(type) {}

void AstValue::print() const {
  std::cout << "AstValue" << std::endl;
}
