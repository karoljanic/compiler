#ifndef AST_VARIABLE_HPP
#define AST_VARIABLE_HPP

#include <iostream>
#include <string>

#include "AstLeftValue.hpp"

class AstVariable : public AstLeftValue {
 private:
  std::string name;

 public:
  AstVariable();
  AstVariable(const std::string name);
  virtual ~AstVariable() = default;
  AstVariable(const AstVariable& from) = default;
  AstVariable& operator=(const AstVariable& from) = default;
  AstVariable(AstVariable&&) = default;
  AstVariable& operator=(AstVariable&&) = default;

  virtual void print() const override;
};

#endif  // AST_VARIABLE_HPP
