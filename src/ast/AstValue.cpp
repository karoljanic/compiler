#include "../../include/ast/AstValue.hpp"

AstValue::AstValue() : AstNode(NodeType::VALUE) {}

AstValue::AstValue(NodeType type) : AstNode(type) {}

std::shared_ptr<AstNode> AstValue::copy(const std::map<std::string, std::string> &/*rewriteTable*/) const {
  return std::make_shared<AstValue>(nodeType);
}

void AstValue::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "value";
}
