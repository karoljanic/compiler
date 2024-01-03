#include "../../include/ast/AstProcedure.hpp"

#include <iostream>

AstProcedure::AstProcedure() : AstNode(NodeType::PROCEDURE) {}

AstProcedure::AstProcedure(std::shared_ptr<AstProcedureHeader> header, std::shared_ptr<AstDeclarations> declarations,
                           std::shared_ptr<AstCommands> commands)
    : AstNode(NodeType::PROCEDURE), header(header), declarations(declarations), commands(commands) {}

const std::shared_ptr<AstProcedureHeader>& AstProcedure::getHeader() {
  return header;
}

const std::shared_ptr<AstDeclarations>& AstProcedure::getDeclarations() {
  return declarations;
}

const std::shared_ptr<AstCommands>& AstProcedure::getCommands() {
  return commands;
}

void AstProcedure::print(std::ostream& out, int tab) const {
  header->print(out, tab);
  out << std::endl;
  declarations->print(out, tab + 4);
  commands->print(out, tab + 4);
  out << std::string(tab, ' ') << "ENDPROC";
}
