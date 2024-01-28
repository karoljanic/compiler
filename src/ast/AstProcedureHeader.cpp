#include "../../include/ast/AstProcedureHeader.hpp"

AstProcedureHeader::AstProcedureHeader() {}

AstProcedureHeader::AstProcedureHeader(const std::string &name, std::shared_ptr<AstArgsDeclaration> argsDeclaration)
	: AstNode(NodeType::PROCEDURE_HEADER), name(name), argsDeclaration(argsDeclaration) {}

const std::string &AstProcedureHeader::getName() const {
  return name;
}

const std::shared_ptr<AstArgsDeclaration> &AstProcedureHeader::getArgsDeclaration() const {
  return argsDeclaration;
}

std::shared_ptr<AstNode> AstProcedureHeader::copy(const std::map<std::string, std::string> &rewriteTable) const {
  std::string newName = name;
  if (rewriteTable.find(name) != rewriteTable.end()) {
	newName = rewriteTable.at(name);
  }

  return std::make_shared<AstProcedureHeader>(newName,
											  std::dynamic_pointer_cast<AstArgsDeclaration>(argsDeclaration->copy(
												  rewriteTable)));
}

void AstProcedureHeader::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "PROC " << name << "( ";
  argsDeclaration->print(out, 0);
  out << " )";
}
