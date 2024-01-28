#include "../../include/ast/AstDeclarations.hpp"

AstDeclarations::AstDeclarations() : AstNode(NodeType::DECLARATIONS) {}

AstDeclarations::AstDeclarations(std::shared_ptr<AstLeftValue> declaration) : AstNode(NodeType::DECLARATIONS) {
  declarations.push_back(declaration);
}

void AstDeclarations::addDeclaration(std::shared_ptr<AstLeftValue> declaration) {
  declarations.push_back(declaration);
}

const std::vector<std::shared_ptr<AstLeftValue>> &AstDeclarations::getDeclarations() const {
  return declarations;
}

std::shared_ptr<AstNode> AstDeclarations::copy(const std::map<std::string, std::string> &rewriteTable) const {
  auto result = std::make_shared<AstDeclarations>();
  for (auto &declaration : declarations) {
	result->addDeclaration(std::dynamic_pointer_cast<AstLeftValue>(declaration->copy(rewriteTable)));
  }
  return result;
}

void AstDeclarations::print(std::ostream &out, int tab) const {
  for (auto &declaration : declarations) {
	declaration->print(out, tab);
	out << std::endl;
  }
}
