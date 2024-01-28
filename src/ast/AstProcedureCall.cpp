#include "../../include/ast/AstProcedureCall.hpp"

std::map<std::string, size_t> AstProcedureCall::functionCallsCounter;

AstProcedureCall::AstProcedureCall() : AstCommand(CommandType::PROCEDURE_CALL) {}

AstProcedureCall::AstProcedureCall(std::string name, std::shared_ptr<AstArgsList> argsList)
	: AstCommand(CommandType::PROCEDURE_CALL), name(name), argsList(argsList) {

  if (AstProcedureCall::functionCallsCounter.find(name) == AstProcedureCall::functionCallsCounter.end()) {
	AstProcedureCall::functionCallsCounter[name] = 0;
  }
  AstProcedureCall::functionCallsCounter[name]++;
}

const std::string &AstProcedureCall::getName() const {
  return name;
}

const std::shared_ptr<AstArgsList> &AstProcedureCall::getArgsList() const {
  return argsList;
}

std::shared_ptr<AstNode> AstProcedureCall::copy(const std::map<std::string, std::string> &rewriteTable) const {
  std::string newName = name;
  if (rewriteTable.find(name) != rewriteTable.end()) {
	newName = rewriteTable.at(name);
  }

  return std::make_shared<AstProcedureCall>(newName,
											std::dynamic_pointer_cast<AstArgsList>(argsList->copy(rewriteTable)));
}

void AstProcedureCall::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "CALL " << name << "(";
  argsList->print(out, 0);
  out << ")";
}

const std::map<std::string, size_t> &AstProcedureCall::getFunctionCallsCounter() {
  return AstProcedureCall::functionCallsCounter;
}