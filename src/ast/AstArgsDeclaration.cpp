#include "../../include/ast/AstArgsDeclaration.hpp"

AstArgsDeclaration::AstArgsDeclaration() : AstNode(NodeType::ARGS_DECLARATION) {}

AstArgsDeclaration::AstArgsDeclaration(std::shared_ptr<AstLeftValue> arg) : AstNode(NodeType::ARGS_DECLARATION) {
  args.push_back(arg);
}

void AstArgsDeclaration::addArg(std::shared_ptr<AstLeftValue> arg) {
  args.push_back(arg);
}

const std::vector<std::shared_ptr<AstLeftValue>> &AstArgsDeclaration::getArgs() const {
  return args;
}

std::shared_ptr<AstNode> AstArgsDeclaration::copy(const std::map<std::string, std::string> &rewriteTable) const {
  auto copy = std::make_shared<AstArgsDeclaration>();
  for (auto &arg : args) {
	copy->addArg(std::dynamic_pointer_cast<AstLeftValue>(arg->copy(rewriteTable)));
  }
  return copy;
}

void AstArgsDeclaration::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "ArgsDeclaration: (";
  for (auto &arg : args) {
	arg->print(out, 0);
	out << ", ";
  }
  out << ")";
}
