#include <iostream>

#include "../../include/ast/AstProcedureCall.hpp"

AstProcedureCall::AstProcedureCall() : AstCommand(CommandType::PROCEDURE_CALL) {}

AstProcedureCall::AstProcedureCall(std::string name, std::shared_ptr<AstArgsList> argsList)
    : AstCommand(CommandType::PROCEDURE_CALL), name(name), argsList(argsList) {}

void AstProcedureCall::print() const {
  std::cout << "ProcedureCall(" << name << ", ";
  if (argsList != nullptr) {
    argsList->print();
  }
  std::cout << ")";
}
