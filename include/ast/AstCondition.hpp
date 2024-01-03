#ifndef AST_CONDITION_HPP
#define AST_CONDITION_HPP

#include <iostream>
#include <memory>

#include "AstNode.hpp"
#include "AstValue.hpp"

class AstCondition : public AstNode {
 public:
  enum ConditionType { EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL };

 private:
  ConditionType conditionType;
  std::shared_ptr<AstValue> left;
  std::shared_ptr<AstValue> right;

 public:
  AstCondition();
  AstCondition(ConditionType conditionType, std::shared_ptr<AstValue> left, std::shared_ptr<AstValue> right);
  virtual ~AstCondition() = default;
  AstCondition(const AstCondition& from) = default;
  AstCondition& operator=(const AstCondition& from) = default;
  AstCondition(AstCondition&&) = default;
  AstCondition& operator=(AstCondition&&) = default;

  virtual void print() const;
};

#endif  // AST_CONDITION_HPP
