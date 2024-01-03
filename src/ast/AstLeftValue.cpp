#include "../../include/ast/AstLeftValue.hpp"

AstLeftValue::AstLeftValue() : AstValue(NodeType::LVALUE) {}

AstLeftValue::AstLeftValue(NodeType type) : AstValue(type) {}

void AstLeftValue::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "lvalue";
}
