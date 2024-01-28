#include "../../include/ast/AstRightValue.hpp"

AstRightValue::AstRightValue() : AstValue(NodeType::RVALUE) {}

AstRightValue::AstRightValue(NodeType type) : AstValue(type) {}

std::shared_ptr<AstNode> AstRightValue::copy(const std::map<std::string, std::string> &/*rewriteTable*/) const {
  return std::make_shared<AstRightValue>(nodeType);
}

void AstRightValue::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "rvalue: ";
}
