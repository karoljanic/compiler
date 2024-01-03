#ifndef LOOKUP_TABLE_HPP
#define LOOKUP_TABLE_HPP

#include <map>
#include <memory>
#include <string>

#include "ast/AstLeftValue.hpp"
#include "ast/AstProcedure.hpp"

class LookupTable {
 private:
  std::map<std::string, std::shared_ptr<AstLeftValue>> variables;
  std::map<std::string, std::shared_ptr<AstProcedure>> functions;

 public:
  LookupTable();
  void addVariable(std::string name, std::shared_ptr<AstLeftValue> value);
  void addFunction(std::string name, std::shared_ptr<AstProcedure> function);

  void removeVariable(std::string name);

  bool nameExists(std::string name);
  bool hasVariable(std::string name);
  bool hasFunction(std::string name);

  std::shared_ptr<AstLeftValue>& getVariable(std::string name);
  std::shared_ptr<AstProcedure>& getFunction(std::string name);
};

#endif  // LOOKUP_TABLE_HPP
