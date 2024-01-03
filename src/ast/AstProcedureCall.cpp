#include "../../include/ast/AstProcedureCall.hpp"

AstProcedureCall::AstProcedureCall() : AstCommand(CommandType::PROCEDURE_CALL) {}

AstProcedureCall::AstProcedureCall(std::string name, std::shared_ptr<AstArgsList> argsList)
    : AstCommand(CommandType::PROCEDURE_CALL), name(name), argsList(argsList) {}

const std::string& AstProcedureCall::getName() const {
  return name;
}

const std::shared_ptr<AstArgsList>& AstProcedureCall::getArgsList() const {
  return argsList;
}

void AstProcedureCall::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "CALL " << name << "(";
  argsList->print(out, 0);
  out << ")";
}
