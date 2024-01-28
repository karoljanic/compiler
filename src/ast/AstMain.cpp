#include "../../include/ast/AstMain.hpp"

AstMain::AstMain() : AstNode(NodeType::MAIN) {}

AstMain::AstMain(std::shared_ptr<AstDeclarations> declarations, std::shared_ptr<AstCommands> commands)
	: AstNode(NodeType::MAIN), declarations(declarations), commands(commands) {}

void AstMain::setDeclarations(std::shared_ptr<AstDeclarations> newDeclarations) {
  declarations = newDeclarations;
}

void AstMain::setCommands(std::shared_ptr<AstCommands> newCommands) {
  commands = newCommands;
}

const std::shared_ptr<AstDeclarations> &AstMain::getDeclarations() const {
  return declarations;
}

const std::shared_ptr<AstCommands> &AstMain::getCommands() const {
  return commands;
}

std::shared_ptr<AstNode> AstMain::copy(const std::map<std::string, std::string> &rewriteTable) const {
  return std::make_shared<AstMain>(std::dynamic_pointer_cast<AstDeclarations>(declarations->copy(rewriteTable)),
								   std::dynamic_pointer_cast<AstCommands>(commands->copy(rewriteTable)));
}

void AstMain::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "MAIN" << std::endl;
  declarations->print(out, tab + 4);
  commands->print(out, tab + 4);
  out << std::string(tab, ' ') << "ENDMAIN" << std::endl;
}
