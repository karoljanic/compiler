#include "../../include/ast/AstArray.hpp"

AstArray::AstArray() : AstLeftValue(NodeType::ARRAY, "", true) {}

AstArray::AstArray(const std::string& name, std::shared_ptr<AstValue> argument)
    : AstLeftValue(NodeType::ARRAY, name, true), argument(argument) {}

const std::string& AstArray::getName() const {
  return name;
}

const std::shared_ptr<AstValue>& AstArray::getArgument() const {
  return argument;
}

void AstArray::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "array " << name << "[";
  if (argument != nullptr) {
    argument->print(out, 0);
  }
  out << "]";
}