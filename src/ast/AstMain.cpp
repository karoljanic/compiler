#include "../../include/ast/AstMain.hpp"

AstMain::AstMain() : AstNode(NodeType::MAIN) {}

AstMain::AstMain(std::shared_ptr<AstDeclarations> declarations, std::shared_ptr<AstCommands> commands)
    : AstNode(NodeType::MAIN), declarations(declarations), commands(commands) {}

const std::shared_ptr<AstDeclarations>& AstMain::getDeclarations() const {
  return declarations;
}

const std::shared_ptr<AstCommands>& AstMain::getCommands() const {
  return commands;
}

void AstMain::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "MAIN" << std::endl;
  declarations->print(out, tab + 4);
  commands->print(out, tab + 4);
  out << std::string(tab, ' ') << "ENDMAIN" << std::endl;
}
