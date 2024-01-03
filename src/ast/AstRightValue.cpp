#include "../../include/ast/AstRightValue.hpp"

AstRightValue::AstRightValue() : AstValue(NodeType::RVALUE) {}

AstRightValue::AstRightValue(NodeType type) : AstValue(type) {}

void AstRightValue::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "rvalue: ";
}
