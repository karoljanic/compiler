#include <iostream>

#include "../../include/ast/AstProcedureHeader.hpp"

AstProcedureHeader::AstProcedureHeader() {}

AstProcedureHeader::AstProcedureHeader(const std::string& name, std::shared_ptr<AstArgsDeclaration> argsDeclaration)
    : AstNode(NodeType::PROCEDURE), name(name), argsDeclaration(argsDeclaration) {}

void AstProcedureHeader::print() const {
  std::cout << "ProcedureHeader: " << name;
  argsDeclaration->print();
  std::cout << std::endl;
}
