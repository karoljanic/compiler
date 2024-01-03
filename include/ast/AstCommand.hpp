#ifndef AST_COMMAND_HPP
#define AST_COMMAND_HPP

#include <memory>

#include "AstNode.hpp"

class AstCommand : public AstNode {
 public:
  enum CommandType { DEFAULT, ASSIGNMENT, IF, WHILE, REPEAT, PROCEDURE_CALL, READ, WRITE };

 protected:
  CommandType commandType;

 public:
  AstCommand();
  AstCommand(CommandType type);
  virtual ~AstCommand() = default;
  AstCommand(const AstCommand& from) = default;
  AstCommand& operator=(const AstCommand& from) = default;
  AstCommand(AstCommand&&) = default;
  AstCommand& operator=(AstCommand&&) = default;

  virtual CommandType getCommandType() const;
  virtual void print() const;
};

#endif  // AST_COMMAND_HPP
