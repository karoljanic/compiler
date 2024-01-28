#include "../../include/ast/AstProcedure.hpp"

#include <iostream>

AstProcedure::AstProcedure() : AstNode(NodeType::PROCEDURE) {}

AstProcedure::AstProcedure(std::shared_ptr<AstProcedureHeader> header, std::shared_ptr<AstDeclarations> declarations,
						   std::shared_ptr<AstCommands> commands)
	: AstNode(NodeType::PROCEDURE), header(header), declarations(declarations), commands(commands) {}

void AstProcedure::setDeclarations(std::shared_ptr<AstDeclarations> declarations) {
  this->declarations = declarations;
}

void AstProcedure::setCommands(std::shared_ptr<AstCommands> commands) {
  this->commands = commands;
}

const std::shared_ptr<AstProcedureHeader> &AstProcedure::getHeader() {
  return header;
}

const std::shared_ptr<AstDeclarations> &AstProcedure::getDeclarations() {
  return declarations;
}

const std::shared_ptr<AstCommands> &AstProcedure::getCommands() {
  return commands;
}

std::shared_ptr<AstNode> AstProcedure::copy(const std::map<std::string, std::string> &rewriteTable) const {
  return std::make_shared<AstProcedure>(std::dynamic_pointer_cast<AstProcedureHeader>(header->copy(rewriteTable)),
										std::dynamic_pointer_cast<AstDeclarations>(declarations->copy(rewriteTable)),
										std::dynamic_pointer_cast<AstCommands>(commands->copy(rewriteTable)));
}

void AstProcedure::print(std::ostream &out, int tab) const {
  header->print(out, tab);
  out << std::endl;
  declarations->print(out, tab + 4);
  commands->print(out, tab + 4);
  out << std::string(tab, ' ') << "ENDPROC";
}
