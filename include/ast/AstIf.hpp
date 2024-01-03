#ifndef AST_IF_HPP
#define AST_IF_HPP

#include <memory>
#include <ostream>

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

  const std::shared_ptr<AstCondition>& getCondition() const;
  const std::shared_ptr<AstCommands>& getCommands() const;
  const std::shared_ptr<AstCommands>& getElseCommands() const;

  void print(std::ostream& out, int tab) const override;
};

#endif  // AST_IF_HPP
