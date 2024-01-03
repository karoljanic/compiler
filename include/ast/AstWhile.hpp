#ifndef AST_WHILE_HPP
#define AST_WHILE_HPP

#include <memory>

#include "AstCommand.hpp"
#include "AstCommands.hpp"
#include "AstCondition.hpp"

class AstWhile : public AstCommand {
 private:
  std::shared_ptr<AstCondition> condition;
  std::shared_ptr<AstCommands> commands;

 public:
  AstWhile();
  AstWhile(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands);
  ~AstWhile() override = default;
  AstWhile(const AstWhile& from) = default;
  AstWhile& operator=(const AstWhile& from) = default;
  AstWhile(AstWhile&&) = default;
  AstWhile& operator=(AstWhile&&) = default;

  std::shared_ptr<AstCondition> getCondition() const;
  std::shared_ptr<AstCommands> getCommands() const;

  void print() const override;
};

#endif  // AST_WHILE_HPP
