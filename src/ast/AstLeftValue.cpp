#include "../../include/ast/AstLeftValue.hpp"

AstLeftValue::AstLeftValue() : AstValue(NodeType::LVALUE) {}

AstLeftValue::AstLeftValue(NodeType type, std::string varName, bool init)
	: AstValue(type), name(varName), initialized(init) {}

void AstLeftValue::initialize() {
  initialized = true;
}

bool AstLeftValue::isInitialized() const {
  return initialized;
}

const std::string &AstLeftValue::getName() const {
  return name;
}

std::shared_ptr<AstNode> AstLeftValue::copy(const std::map<std::string, std::string> &rewriteTable) const {
  std::string newName = name;
  if (rewriteTable.find(name) != rewriteTable.end()) {
	newName = rewriteTable.at(name);
  }
  return std::make_shared<AstLeftValue>(nodeType, newName, initialized);
}

void AstLeftValue::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "lvalue";
}
