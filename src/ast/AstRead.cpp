#include "../../include/ast/AstRead.hpp"

AstRead::AstRead() : AstCommand(CommandType::READ) {}

AstRead::AstRead(std::shared_ptr<AstLeftValue> lvalue) : AstCommand(CommandType::READ), lvalue(lvalue) {}

const std::shared_ptr<AstLeftValue> &AstRead::getLvalue() const {
  return lvalue;
}

std::shared_ptr<AstNode> AstRead::copy(const std::map<std::string, std::string> &rewriteTable) const {
  return std::make_shared<AstRead>(std::dynamic_pointer_cast<AstLeftValue>(lvalue->copy(rewriteTable)));
}

void AstRead::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "READ ";
  lvalue->print(out, 0);
}
