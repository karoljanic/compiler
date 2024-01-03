#include "../../include/ast/AstProcedures.hpp"

AstProcedures::AstProcedures() : AstNode(NodeType::PROCEDURES) {}

AstProcedures::AstProcedures(std::shared_ptr<AstProcedure> procedure) : AstNode(NodeType::PROCEDURES) {
  procedures.push_back(procedure);
}

void AstProcedures::addProcedure(std::shared_ptr<AstProcedure> procedure) {
  procedures.push_back(procedure);
}

const std::vector<std::shared_ptr<AstProcedure>>& AstProcedures::getProcedures() const {
  return procedures;
}

void AstProcedures::print(std::ostream& out, int tab) const {
  for (auto procedure : procedures) {
    procedure->print(out, tab);
    out << std::endl;
  }
}
