#ifndef AST_LEFT_VALUE_HPP
#define AST_LEFT_VALUE_HPP

#include "AstValue.hpp"

class AstLeftValue : public AstValue {
 public:
  AstLeftValue();
  AstLeftValue(NodeType type);
  virtual ~AstLeftValue() = default;
  AstLeftValue(const AstLeftValue& from) = default;
  AstLeftValue& operator=(const AstLeftValue& from) = default;
  AstLeftValue(AstLeftValue&&) = default;
  AstLeftValue& operator=(AstLeftValue&&) = default;

  virtual void print() const;
};

#endif  // AST_LEFT_VALUE_HPP
