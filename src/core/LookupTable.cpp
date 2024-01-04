#include "../../include/core/LookupTable.hpp"

LookupTable::LookupTable() {}

void LookupTable::addVariable(std::string name, AstVariable* variable) {
  variables[name] = std::make_shared<AstVariable>(*variable);
}

void LookupTable::addArray(std::string name, AstArray* array) {
  arrays[name] = std::make_shared<AstArray>(*array);
}

void LookupTable::addProcedure(std::string name, AstProcedure* procedure) {
  procedures[name] = std::make_shared<AstProcedure>(*procedure);
}

void LookupTable::addLeftValue(std::string name, AstLeftValue* leftValue) {
  if (leftValue->getNodeType() == AstLeftValue::NodeType::ARRAY) {
    addArray(name, dynamic_cast<AstArray*>(leftValue));
  }
  else if (leftValue->getNodeType() == AstLeftValue::NodeType::VARIABLE) {
    addVariable(name, dynamic_cast<AstVariable*>(leftValue));
  }
}

void LookupTable::addScope(std::string scope) {
  scopes.push(scope);
}

void LookupTable::removeVariable(std::string name) {
  variables.erase(name);
}

void LookupTable::removeArray(std::string name) {
  arrays.erase(name);
}

void LookupTable::removeProcedure(std::string name) {
  procedures.erase(name);
}

void LookupTable::removeLeftValue(std::string name) {
  if (hasVariable(name)) {
    removeVariable(name);
  }
  else if (hasArray(name)) {
    removeArray(name);
  }
}

const std::string LookupTable::removeTopScope() {
  std::string scope = scopes.top();
  scopes.pop();
  return scope;
}

bool LookupTable::nameExists(std::string name) const {
  return hasVariable(name) || hasArray(name) || hasProcedure(name);
}

bool LookupTable::hasVariable(std::string name) const {
  return variables.find(name) != variables.end();
}

bool LookupTable::hasArray(std::string name) const {
  return arrays.find(name) != arrays.end();
}

bool LookupTable::hasProcedure(std::string name) const {
  return procedures.find(name) != procedures.end();
}

const std::shared_ptr<AstVariable>& LookupTable::getVariable(std::string name) const {
  return variables.at(name);
}

const std::shared_ptr<AstArray>& LookupTable::getArray(std::string name) const {
  return arrays.at(name);
}

const std::shared_ptr<AstProcedure>& LookupTable::getProcedure(std::string name) const {
  return procedures.at(name);
}

const std::shared_ptr<AstLeftValue> LookupTable::getLeftValue(AstLeftValue* leftValue) const {
  if (leftValue->getNodeType() == AstLeftValue::NodeType::ARRAY) {
    return getArray(leftValue->getName());
  }
  else if (leftValue->getNodeType() == AstLeftValue::NodeType::VARIABLE) {
    return getVariable(leftValue->getName());
  }

  return nullptr;
}

const std::string& LookupTable::getTopScope() const {
  return scopes.top();
}