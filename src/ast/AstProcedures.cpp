#include <iostream>

#include "../../include/ast/AstProcedures.hpp"

AstProcedures::AstProcedures() : AstNode(NodeType::PROCEDURES) {}

AstProcedures::AstProcedures(std::shared_ptr<AstProcedure> procedure) : AstNode(NodeType::PROCEDURES) {
  procedures.push_back(procedure);
}

void AstProcedures::addProcedure(std::shared_ptr<AstProcedure> procedure) {
  procedures.push_back(procedure);
}

std::vector<std::shared_ptr<AstProcedure>>& AstProcedures::getProcedures() {
  return procedures;
}

void AstProcedures::print() const {
  std::cout << "Procedures: " << std::endl;
  for (auto procedure : procedures) {
    procedure->print();
  }
}
