#include "../../include/ast/AstArgsList.hpp"

AstArgsList::AstArgsList() : AstNode(NodeType::ARGS_LIST) {}

AstArgsList::AstArgsList(std::shared_ptr<AstLeftValue> arg) : AstNode(NodeType::ARGS_LIST) {
  args.push_back(arg);
}

void AstArgsList::addArg(std::shared_ptr<AstLeftValue> arg) {
  args.push_back(arg);
}

const std::vector<std::shared_ptr<AstLeftValue>>& AstArgsList::getArgs() const {
  return args;
}

void AstArgsList::print(std::ostream& out, int /*tab*/) const {
  out << "ArgsList: (";
  for (auto& arg : args) {
    arg->print(out, 0);
    out << ", ";
  }
  out << ")";
}
