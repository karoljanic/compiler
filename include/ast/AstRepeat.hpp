#ifndef AST_REPEAT_HPP
#define AST_REPEAT_HPP

#include <memory>
#include <ostream>

#include "AstCommand.hpp"
#include "AstCommands.hpp"
#include "AstCondition.hpp"

class AstRepeat : public AstCommand {
 private:
  std::shared_ptr<AstCondition> condition;
  std::shared_ptr<AstCommands> commands;

 public:
  AstRepeat();
  AstRepeat(std::shared_ptr<AstCondition> condition, std::shared_ptr<AstCommands> commands);
  ~AstRepeat() override = default;
  AstRepeat(const AstRepeat& from) = default;
  AstRepeat& operator=(const AstRepeat& from) = default;
  AstRepeat(AstRepeat&&) = default;
  AstRepeat& operator=(AstRepeat&&) = default;

  const std::shared_ptr<AstCondition>& getCondition() const;
  const std::shared_ptr<AstCommands>& getCommands() const;

  void print(std::ostream& out, int tab) const override;
};

#endif  // AST_REPEAT_HPP
