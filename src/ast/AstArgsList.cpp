#include <iostream>

#include "../../include/ast/AstArgsList.hpp"

AstArgsList::AstArgsList() : AstNode(NodeType::ARGS_LIST) {}

AstArgsList::AstArgsList(std::shared_ptr<AstLeftValue> arg) : AstNode(NodeType::ARGS_LIST) {
  args.push_back(arg);
}

void AstArgsList::addArg(std::shared_ptr<AstLeftValue> arg) {
  args.push_back(arg);
}

void AstArgsList::print() const {
  std::cout << "ArgsList: (";
  for (auto& arg : args) {
    arg->print();
    std::cout << ", ";
  }
  std::cout << ")" << std::endl;
}
