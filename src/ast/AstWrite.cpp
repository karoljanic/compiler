#include "../../include/ast/AstWrite.hpp"

AstWrite::AstWrite() : AstCommand(CommandType::WRITE) {}

AstWrite::AstWrite(std::shared_ptr<AstValue> value) : AstCommand(CommandType::WRITE), value(value) {}

const std::shared_ptr<AstValue> &AstWrite::getValue() const {
  return value;
}

std::shared_ptr<AstNode> AstWrite::copy(const std::map<std::string, std::string> &rewriteTable) const {
  return std::make_shared<AstWrite>(std::dynamic_pointer_cast<AstValue>(value->copy(rewriteTable)));
}

void AstWrite::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "WRITE ";
  value->print(out, 0);
}
