#include <iostream>

#include "../../include/ast/AstRead.hpp"

AstRead::AstRead() : AstCommand(CommandType::READ) {}

AstRead::AstRead(std::shared_ptr<AstLeftValue> lvalue) : AstCommand(CommandType::READ), lvalue(lvalue) {}

void AstRead::print() const {
  std::cout << "AstRead {" << std::endl;
  std::cout << "lvalue: ";
  lvalue->print();
  std::cout << "}" << std::endl;
}
