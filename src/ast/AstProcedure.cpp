#include <iostream>

#include "../../include/ast/AstProcedure.hpp"

AstProcedure::AstProcedure() : AstNode(NodeType::PROCEDURE) {}

AstProcedure::AstProcedure(std::shared_ptr<AstProcedureHeader> header, std::shared_ptr<AstDeclarations> declarations,
                           std::shared_ptr<AstCommands> commands)
    : AstNode(NodeType::PROCEDURE), header(header), declarations(declarations), commands(commands) {}

std::shared_ptr<AstProcedureHeader>& AstProcedure::getHeader() {
  return header;
}

std::shared_ptr<AstDeclarations>& AstProcedure::getDeclarations() {
  return declarations;
}

std::shared_ptr<AstCommands>& AstProcedure::getCommands() {
  return commands;
}

void AstProcedure::print() const {
  std::cout << "Procedure: " << std::endl;
  header->print();
  declarations->print();
  commands->print();
}
