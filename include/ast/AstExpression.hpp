#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include <iostream>
#include <memory>
#include <ostream>

#include "AstNode.hpp"
#include "AstValue.hpp"

class AstExpression : public AstNode {
 public:
  enum ExpressionType { VALUE, ADD, SUB, MUL, DIV, MOD };

 private:
  ExpressionType expressionType;
  std::shared_ptr<AstValue> left;
  std::shared_ptr<AstValue> right;

 public:
  AstExpression();
  AstExpression(ExpressionType type, std::shared_ptr<AstValue> left);
  AstExpression(ExpressionType type, std::shared_ptr<AstValue> left, std::shared_ptr<AstValue> right);
  virtual ~AstExpression() = default;
  AstExpression(const AstExpression &from) = default;
  AstExpression &operator=(const AstExpression &from) = default;
  AstExpression(AstExpression &&) = default;
  AstExpression &operator=(AstExpression &&) = default;

  ExpressionType getExpressionType() const;
  const std::shared_ptr<AstValue> &getLeft() const;
  const std::shared_ptr<AstValue> &getRight() const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  virtual void print(std::ostream &out, int tab) const override;
};

#endif  // AST_EXPRESSION_HPP
