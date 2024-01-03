#ifndef AST_ASSIGNMENT_HPP
#define AST_ASSIGNMENT_HPP

#include <memory>

#include "AstCommand.hpp"
#include "AstExpression.hpp"
#include "AstLeftValue.hpp"

class AstAssignment : public AstCommand {
 private:
  std::shared_ptr<AstLeftValue> lvalue;
  std::shared_ptr<AstExpression> expression;

 public:
  AstAssignment();
  AstAssignment(std::shared_ptr<AstLeftValue> lvalue, std::shared_ptr<AstExpression> expression);
  virtual ~AstAssignment() = default;
  AstAssignment(const AstAssignment& from) = default;
  AstAssignment& operator=(const AstAssignment& from) = default;
  AstAssignment(AstAssignment&&) = default;
  AstAssignment& operator=(AstAssignment&&) = default;

  virtual void print() const;
};

#endif  // AST_ASSIGNMENT_HPP
