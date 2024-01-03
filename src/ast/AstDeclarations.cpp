#include <iostream>

#include "../../include/ast/AstDeclarations.hpp"

AstDeclarations::AstDeclarations() : AstNode(NodeType::DECLARATIONS) {}

AstDeclarations::AstDeclarations(std::shared_ptr<AstLeftValue> declaration) : AstNode(NodeType::DECLARATIONS) {
  declarations.push_back(declaration);
}

void AstDeclarations::addDeclaration(std::shared_ptr<AstLeftValue> declaration) {
  declarations.push_back(declaration);
}

void AstDeclarations::print() const {
  std::cout << "Declarations: " << std::endl;
  for (auto& declaration : declarations) {
    declaration->print();
  }
}
