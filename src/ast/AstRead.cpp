#include "../../include/ast/AstRead.hpp"

AstRead::AstRead() : AstCommand(CommandType::READ) {}

AstRead::AstRead(std::shared_ptr<AstLeftValue> lvalue) : AstCommand(CommandType::READ), lvalue(lvalue) {}

const std::shared_ptr<AstLeftValue>& AstRead::getLvalue() const {
  return lvalue;
}

void AstRead::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "READ ";
  lvalue->print(out, 0);
}
