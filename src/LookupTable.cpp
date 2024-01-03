#include "../include/LookupTable.hpp"

LookupTable::LookupTable() {}

void LookupTable::addVariable(std::string name, std::shared_ptr<AstLeftValue> value) {
  variables[name] = value;
}

void LookupTable::addFunction(std::string name, std::shared_ptr<AstProcedure> function) {
  functions[name] = function;
}

void LookupTable::removeVariable(std::string name) {
  variables.erase(name);
}

bool LookupTable::nameExists(std::string name) {
  return hasVariable(name) || hasFunction(name);
}

bool LookupTable::hasVariable(std::string name) {
  return variables.find(name) != variables.end();
}

bool LookupTable::hasFunction(std::string name) {
  return functions.find(name) != functions.end();
}

std::shared_ptr<AstLeftValue>& LookupTable::getVariable(std::string name) {
  return variables[name];
}

std::shared_ptr<AstProcedure>& LookupTable::getFunction(std::string name) {
  return functions[name];
}
