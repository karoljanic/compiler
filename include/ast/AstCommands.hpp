#ifndef AST_COMMANDS_HPP
#define AST_COMMANDS_HPP

#include <memory>
#include <ostream>
#include <vector>

#include "AstCommand.hpp"
#include "AstNode.hpp"

class AstCommands : public AstNode {
 private:
  std::vector<std::shared_ptr<AstCommand>> commands;

 public:
  AstCommands();
  AstCommands(std::shared_ptr<AstCommand> command);
  virtual ~AstCommands() = default;
  AstCommands(const AstCommands& from) = default;
  AstCommands& operator=(const AstCommands& from) = default;
  AstCommands(AstCommands&&) = default;
  AstCommands& operator=(AstCommands&&) = default;

  void addCommand(std::shared_ptr<AstCommand> command);
  const std::vector<std::shared_ptr<AstCommand>>& getCommands();

  void print(std::ostream& out, int tab) const override;
};

#endif  // AST_COMMANDS_HPP
