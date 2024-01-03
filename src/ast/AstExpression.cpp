#include <iostream>

#include "../../include/ast/AstExpression.hpp"

AstExpression::AstExpression() : AstNode(NodeType::EXPRESSION) {}

AstExpression::AstExpression(ExpressionType type, std::shared_ptr<AstValue> left)
    : AstNode(NodeType::EXPRESSION), expressionType(type), left(left) {}

AstExpression::AstExpression(ExpressionType type, std::shared_ptr<AstValue> left, std::shared_ptr<AstValue> right)
    : AstNode(NodeType::EXPRESSION), expressionType(type), left(left), right(right) {}

void AstExpression::print() const {
  left->print();
  switch (expressionType) {
    case VALUE:
      return;
    case ADD:
      std::cout << " + ";
      break;
    case SUB:
      std::cout << " - ";
      break;
    case MUL:
      std::cout << " * ";
      break;
    case DIV:
      std::cout << " / ";
      break;
    case MOD:
      std::cout << " % ";
      break;
  }
  right->print();
}
