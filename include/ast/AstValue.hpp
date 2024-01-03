#ifndef AST_VALUE_HPP
#define AST_VALUE_HPP

#include <ostream>

#include "AstNode.hpp"

class AstValue : public AstNode {
 public:
  AstValue();
  AstValue(NodeType type);
  virtual ~AstValue() = default;
  AstValue(const AstValue& from) = default;
  AstValue& operator=(const AstValue& from) = default;
  AstValue(AstValue&&) = default;
  AstValue& operator=(AstValue&&) = default;

  virtual void print(std::ostream& out, int tab) const;
};

#endif  // AST_VALUE_HPP
