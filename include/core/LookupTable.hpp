#ifndef LOOKUP_TABLE_HPP
#define LOOKUP_TABLE_HPP

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <utility>

#include "../ast/AstArray.hpp"
#include "../ast/AstProcedure.hpp"
#include "../ast/AstVariable.hpp"

class LookupTable {
 private:
  std::map<std::string, std::shared_ptr<AstVariable>> variables;
  std::map<std::string, std::shared_ptr<AstArray>> arrays;
  std::map<std::string, std::shared_ptr<AstProcedure>> procedures;
  std::stack<std::string> scopes;

 public:
  LookupTable();

  void addVariable(std::string name, AstVariable* variable);
  void addArray(std::string name, AstArray* array);
  void addProcedure(std::string name, AstProcedure* procedure);
  void addLeftValue(std::string name, AstLeftValue* leftValue);
  void addScope(std::string scope);

  void removeVariable(std::string name);
  void removeArray(std::string name);
  void removeProcedure(std::string name);
  void removeLeftValue(std::string name);
  const std::string removeTopScope();

  bool nameExists(std::string name) const;
  bool hasVariable(std::string name) const;
  bool hasArray(std::string name) const;
  bool hasProcedure(std::string name) const;

  const std::shared_ptr<AstVariable>& getVariable(std::string name) const;
  const std::shared_ptr<AstArray>& getArray(std::string name) const;
  const std::shared_ptr<AstProcedure>& getProcedure(std::string name) const;
  const std::shared_ptr<AstLeftValue> getLeftValue(AstLeftValue* leftValue) const;
  const std::string& getTopScope() const;
};

#endif  // LOOKUP_TABLE_HPP
