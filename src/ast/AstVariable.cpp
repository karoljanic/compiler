#include "../../include/ast/AstVariable.hpp"

AstVariable::AstVariable() : AstLeftValue(NodeType::VARIABLE, "", false) {}

AstVariable::AstVariable(const std::string name) : AstLeftValue(NodeType::VARIABLE, name, false) {}

const std::string &AstVariable::getName() const {
  return name;
}

std::shared_ptr<AstNode> AstVariable::copy(const std::map<std::string, std::string> &rewriteTable) const {
  std::string newName = name;
  if (rewriteTable.find(name) != rewriteTable.end()) {
	newName = rewriteTable.at(name);
  }
  return std::make_shared<AstVariable>(newName);
}

void AstVariable::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "var " << name;
}
