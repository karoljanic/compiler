#ifndef AST_NODE_HPP
#define AST_NODE_HPP

class AstNode {
 public:
  enum Type { NUMBER, VARIABLE, ARRAY, EXPRESSION, CONDITION, COMMAND };

  AstNode() {}
};

#endif  // AST_NODE_HPP