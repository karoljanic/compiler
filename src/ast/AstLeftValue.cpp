#include "../../include/ast/AstLeftValue.hpp"

AstLeftValue::AstLeftValue() : AstValue(NodeType::LVALUE) {}

AstLeftValue::AstLeftValue(NodeType type, std::string varName, bool init) : AstValue(type), name(varName), initialized(init) {}

void AstLeftValue::initialize() {
  initialized = true;
}

bool AstLeftValue::isInitialized() const {
  return initialized;
}

const std::string& AstLeftValue::getName() const {
  return name;
}

void AstLeftValue::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "lvalue";
}
