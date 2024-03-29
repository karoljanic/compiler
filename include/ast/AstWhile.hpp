#ifndef AST_WHILE_HPP
#define AST_WHILE_HPP

#include <memory>
#include <ostream>

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
  AstWhile(const AstWhile &from) = default;
  AstWhile &operator=(const AstWhile &from) = default;
  AstWhile(AstWhile &&) = default;
  AstWhile &operator=(AstWhile &&) = default;

  const std::shared_ptr<AstCondition> &getCondition() const;
  const std::shared_ptr<AstCommands> &getCommands() const;

  void setCondition(const std::shared_ptr<AstCondition> &condition);
  void setCommands(const std::shared_ptr<AstCommands> &commands);

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  void print(std::ostream &out, int tab) const override;
};

#endif  // AST_WHILE_HPP
