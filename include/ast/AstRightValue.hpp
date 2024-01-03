#ifndef AST_RIGHT_VALUE_HPP
#define AST_RIGHT_VALUE_HPP

#include <ostream>

#include "AstValue.hpp"

class AstRightValue : public AstValue {
 public:
  AstRightValue();
  AstRightValue(NodeType type);
  virtual ~AstRightValue() = default;
  AstRightValue(const AstRightValue& from) = default;
  AstRightValue& operator=(const AstRightValue& from) = default;
  AstRightValue(AstRightValue&&) = default;
  AstRightValue& operator=(AstRightValue&&) = default;

  virtual void print(std::ostream& out, int tab) const;
};

#endif  // AST_RIGHT_VALUE_HPP
