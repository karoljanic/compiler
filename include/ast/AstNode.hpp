#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <ostream>
#include <map>
#include <memory>
#include <iostream>

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
  AstNode(const AstNode &from) = default;
  AstNode &operator=(const AstNode &from) = default;
  AstNode(AstNode &&) = default;
  AstNode &operator=(AstNode &&) = default;

  virtual NodeType getNodeType() const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const;
  virtual void print(std::ostream &out, int tab) const;
};

#endif  // AST_NODE_HPP
