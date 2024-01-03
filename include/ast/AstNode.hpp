#ifndef AST_NODE_HPP
#define AST_NODE_HPP

class AstNode {
 public:
  enum NodeType {
    DEFAULT,
    VALUE,
    LVALUE,
    RVALUE,
    NUMBER,
    VARIABLE,
    ARRAY,
    EXPRESSION,
    CONDITION,
    ARGS_LIST,
    ARGS_DECLARATION,
    DECLARATIONS,
    PROCEDURE_HEADER,
    PROCEDURE,
    PROCEDURES,
    COMMAND,
    COMMANDS,
    MAIN,
    PROGRAM
  };

 protected:
  NodeType nodeType;

 public:
  AstNode();
  AstNode(NodeType type);
  virtual ~AstNode() = default;
  AstNode(const AstNode& from) = default;
  AstNode& operator=(const AstNode& from) = default;
  AstNode(AstNode&&) = default;
  AstNode& operator=(AstNode&&) = default;

  virtual NodeType getNodeType() const;
  virtual void print() const;
};

#endif  // AST_NODE_HPP
