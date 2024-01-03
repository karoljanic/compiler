#include "../../include/ast/AstDeclarations.hpp"

AstDeclarations::AstDeclarations() : AstNode(NodeType::DECLARATIONS) {}

AstDeclarations::AstDeclarations(std::shared_ptr<AstLeftValue> declaration) : AstNode(NodeType::DECLARATIONS) {
  declarations.push_back(declaration);
}

void AstDeclarations::addDeclaration(std::shared_ptr<AstLeftValue> declaration) {
  declarations.push_back(declaration);
}

const std::vector<std::shared_ptr<AstLeftValue>>& AstDeclarations::getDeclarations() const {
  return declarations;
}

void AstDeclarations::print(std::ostream& out, int tab) const {
  for (auto& declaration : declarations) {
    declaration->print(out, tab);
    out << std::endl;
  }
}
