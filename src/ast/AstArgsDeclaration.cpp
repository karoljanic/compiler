#include "../../include/ast/AstArgsDeclaration.hpp"
#include <iostream>

AstArgsDeclaration::AstArgsDeclaration() : AstNode(NodeType::ARGS_LIST) {}

AstArgsDeclaration::AstArgsDeclaration(std::shared_ptr<AstLeftValue> arg) : AstNode(NodeType::ARGS_LIST) {
  args.push_back(arg);
}

void AstArgsDeclaration::addArg(std::shared_ptr<AstLeftValue> arg) {
  args.push_back(arg);
}

void AstArgsDeclaration::print() const {
  std::cout << "ArgsDeclaration: (";
  for (auto& arg : args) {
    arg->print();
    std::cout << ", ";
  }
  std::cout << ")" << std::endl;
}
