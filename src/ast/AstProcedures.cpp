#include "../../include/ast/AstProcedures.hpp"

AstProcedures::AstProcedures() : AstNode(NodeType::PROCEDURES) {}

AstProcedures::AstProcedures(std::shared_ptr<AstProcedure> procedure) : AstNode(NodeType::PROCEDURES) {
  procedures.push_back(procedure);
}

void AstProcedures::addProcedure(std::shared_ptr<AstProcedure> procedure) {
  procedures.push_back(procedure);
}

const std::vector<std::shared_ptr<AstProcedure>> &AstProcedures::getProcedures() const {
  return procedures;
}

const std::shared_ptr<AstProcedure> &AstProcedures::getProcedure(const std::string &name) const {
  for (const auto &procedure : procedures) {
	if (procedure->getHeader()->getName() == name) {
	  return procedure;
	}
  }
  throw std::runtime_error("Procedure " + name + " not found");
}

std::shared_ptr<AstNode> AstProcedures::copy(const std::map<std::string, std::string> &rewriteTable) const {
  std::shared_ptr<AstProcedures> copy = std::make_shared<AstProcedures>();
  for (auto procedure : procedures) {
	copy->addProcedure(std::dynamic_pointer_cast<AstProcedure>(procedure->copy(rewriteTable)));
  }
  return copy;
}

void AstProcedures::print(std::ostream &out, int tab) const {
  for (auto procedure : procedures) {
	procedure->print(out, tab);
	out << std::endl;
  }
}
