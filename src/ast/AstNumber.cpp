#include "../../include/ast/AstNumber.hpp"

AstNumber::AstNumber() : AstRightValue(NodeType::NUMBER) {}

AstNumber::AstNumber(uint64_t val) : AstRightValue(NodeType::NUMBER), value(val) {}

uint64_t AstNumber::getValue() const {
  return value;
}

std::shared_ptr<AstNode> AstNumber::copy(const std::map<std::string, std::string> & /*rewriteTable*/) const {
  return std::make_shared<AstNumber>(value);
}

void AstNumber::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "num " << value;
}
