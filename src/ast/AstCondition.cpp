#include <iostream>

#include "../../include/ast/AstCondition.hpp"

AstCondition::AstCondition() : AstNode(NodeType::CONDITION) {}

AstCondition::AstCondition(ConditionType conditionType, std::shared_ptr<AstValue> left, std::shared_ptr<AstValue> right)
    : AstNode(NodeType::CONDITION), conditionType(conditionType), left(left), right(right) {}

void AstCondition::print() const {
  left->print();
  switch (conditionType) {
    case EQUAL:
      std::cout << " == ";
      break;
    case NOT_EQUAL:
      std::cout << " != ";
      break;
    case LESS:
      std::cout << " < ";
      break;
    case GREATER:
      std::cout << " > ";
      break;
    case LESS_EQUAL:
      std::cout << " <= ";
      break;
    case GREATER_EQUAL:
      std::cout << " >= ";
      break;
  }
  right->print();
}
