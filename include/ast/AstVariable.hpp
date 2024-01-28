#ifndef AST_VARIABLE_HPP
#define AST_VARIABLE_HPP

#include <ostream>
#include <string>

#include "AstLeftValue.hpp"

class AstVariable : public AstLeftValue {
 public:
  AstVariable();
  AstVariable(const std::string name);
  virtual ~AstVariable() = default;
  AstVariable(const AstVariable &from) = default;
  AstVariable &operator=(const AstVariable &from) = default;
  AstVariable(AstVariable &&) = default;
  AstVariable &operator=(AstVariable &&) = default;

  const std::string &getName() const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  virtual void print(std::ostream &out, int tab) const override;
};

#endif  // AST_VARIABLE_HPP
