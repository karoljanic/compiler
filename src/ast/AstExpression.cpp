#include "../../include/ast/AstExpression.hpp"

AstExpression::AstExpression() : AstNode(NodeType::EXPRESSION) {}

AstExpression::AstExpression(ExpressionType type, std::shared_ptr<AstValue> left)
    : AstNode(NodeType::EXPRESSION), expressionType(type), left(left) {}

AstExpression::AstExpression(ExpressionType type, std::shared_ptr<AstValue> left, std::shared_ptr<AstValue> right)
    : AstNode(NodeType::EXPRESSION), expressionType(type), left(left), right(right) {}

AstExpression::ExpressionType AstExpression::getExpressionType() const {
  return expressionType;
}

const std::shared_ptr<AstValue>& AstExpression::getLeft() const {
  return left;
}

const std::shared_ptr<AstValue>& AstExpression::getRight() const {
  return right;
}

void AstExpression::print(std::ostream& out, int tab) const {
  left->print(out, tab);
  switch (expressionType) {
    case VALUE:
      return;
    case ADD:
      out << " + ";
      break;
    case SUB:
      out << " - ";
      break;
    case MUL:
      out << " * ";
      break;
    case DIV:
      out << " / ";
      break;
    case MOD:
      out << " % ";
      break;
  }
  right->print(out, 0);
}
