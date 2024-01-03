#include <iostream>

#include "../../include/ast/AstMain.hpp"

AstMain::AstMain() : AstNode(NodeType::MAIN) {}

AstMain::AstMain(std::shared_ptr<AstDeclarations> declarations, std::shared_ptr<AstCommands> commands)
    : AstNode(NodeType::MAIN), declarations(declarations), commands(commands) {}

void AstMain::print() const {
  std::cout << "Main" << std::endl;
  declarations->print();
  commands->print();
}
