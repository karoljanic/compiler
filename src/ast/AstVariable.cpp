#include "../../include/ast/AstVariable.hpp"

AstVariable::AstVariable() : AstLeftValue(NodeType::VARIABLE, "", false) {}

AstVariable::AstVariable(const std::string name) : AstLeftValue(NodeType::VARIABLE, name, false) {}

const std::string& AstVariable::getName() const {
  return name;
}

void AstVariable::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "var " << name;
}
