#ifndef AST_IF_HPP
#define AST_IF_HPP

#include <memory>

#include "AstCommand.hpp"
#include "AstCommands.hpp"
#include "AstCondition.hpp"

class AstIf : public AstCommand {
 private:
  std::shared_ptr<AstCondition> condition;
  std::shared_ptr<AstCommands> commands;
  std::shared_ptr<AstCommands> elseCommands;

 public:
  AstIf();
  AstIf(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands,
        std::shared_ptr<AstCommands> elseCommands);
  ~AstIf() override = default;
  AstIf(const AstIf& from) = default;
  AstIf& operator=(const AstIf& from) = default;
  AstIf(AstIf&&) = default;
  AstIf& operator=(AstIf&&) = default;

  std::shared_ptr<AstCondition> getCondition() const;
  std::shared_ptr<AstCommands> getCommands() const;
  std::shared_ptr<AstCommands> getElseCommands() const;

  void print() const override;
};

#endif  // AST_IF_HPP
