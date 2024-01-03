#include <iostream>

#include "../../include/ast/AstNumber.hpp"

AstNumber::AstNumber() : AstRightValue(NodeType::RVALUE) {}

AstNumber::AstNumber(uint64_t val) : AstRightValue(NodeType::RVALUE), value(val) {}

void AstNumber::print() const {
  std::cout << value;
}
