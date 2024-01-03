#include "../../include/ast/AstWrite.hpp"

AstWrite::AstWrite() : AstCommand(CommandType::WRITE) {}

AstWrite::AstWrite(std::shared_ptr<AstValue> value) : AstCommand(CommandType::WRITE), value(value) {}

const std::shared_ptr<AstValue>& AstWrite::getValue() const {
  return value;
}

void AstWrite::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "WRITE ";
  value->print(out, 0);
}
