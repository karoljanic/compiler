#include "../../include/ast/AstProcedureHeader.hpp"

AstProcedureHeader::AstProcedureHeader() {}

AstProcedureHeader::AstProcedureHeader(const std::string& name, std::shared_ptr<AstArgsDeclaration> argsDeclaration)
    : AstNode(NodeType::PROCEDURE_HEADER), name(name), argsDeclaration(argsDeclaration) {}

const std::string& AstProcedureHeader::getName() const {
  return name;
}

const std::shared_ptr<AstArgsDeclaration>& AstProcedureHeader::getArgsDeclaration() const {
  return argsDeclaration;
}

void AstProcedureHeader::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "PROC " << name << "( ";
  argsDeclaration->print(out, 0);
  out << " )";
}
