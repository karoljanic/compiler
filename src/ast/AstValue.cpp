#include "../../include/ast/AstValue.hpp"

AstValue::AstValue() : AstNode(NodeType::VALUE) {}

AstValue::AstValue(NodeType type) : AstNode(type) {}

void AstValue::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "value";
}
