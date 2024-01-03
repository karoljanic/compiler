#include <iostream>

#include "../../include/ast/AstArray.hpp"

AstArray::AstArray() : AstLeftValue(NodeType::ARRAY) {}

AstArray::AstArray(const std::string& name, std::shared_ptr<AstValue> argument)
    : AstLeftValue(NodeType::ARRAY), name(name), argument(argument) {}

void AstArray::print() const {
  std::cout << name << "[";
  if (argument != nullptr) {
    argument->print();
  }
  std::cout << "]";
}