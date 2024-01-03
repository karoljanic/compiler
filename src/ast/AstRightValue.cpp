#include <iostream>

#include "../../include/ast/AstRightValue.hpp"

AstRightValue::AstRightValue() : AstValue(NodeType::RVALUE) {}

AstRightValue::AstRightValue(NodeType type) : AstValue(type) {}

void AstRightValue::print() const {
  std::cout << "AstRightValue" << std::endl;
}
