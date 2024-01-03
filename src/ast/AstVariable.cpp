#include "../../include/ast/AstVariable.hpp"

AstVariable::AstVariable() : AstLeftValue(NodeType::VARIABLE) {}

AstVariable::AstVariable(const std::string name) : AstLeftValue(NodeType::VARIABLE), name(name) {}

const std::string& AstVariable::getName() const {
  return name;
}

void AstVariable::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "var " << name;
}
