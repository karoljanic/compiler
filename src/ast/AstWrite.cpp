#include <iostream>

#include "../../include/ast/AstWrite.hpp"

AstWrite::AstWrite() : AstCommand(CommandType::WRITE) {}

AstWrite::AstWrite(std::shared_ptr<AstValue> value) : AstCommand(CommandType::WRITE), value(value) {}

void AstWrite::print() const {
  std::cout << "WRITE ";
  value->print();
  std::cout << std::endl;
}
