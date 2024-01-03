#ifndef AST_CONDITION_HPP
#define AST_CONDITION_HPP

#include <iostream>
#include <memory>
#include <ostream>

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

  ConditionType getConditionType() const;
  const std::shared_ptr<AstValue>& getLeft() const;
  const std::shared_ptr<AstValue>& getRight() const;

  virtual void print(std::ostream& out, int tab) const;
};

#endif  // AST_CONDITION_HPP
