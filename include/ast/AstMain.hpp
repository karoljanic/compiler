#ifndef AST_MAIN_HPP
#define AST_MAIN_HPP

#include <memory>

#include "AstCommands.hpp"
#include "AstDeclarations.hpp"
#include "AstNode.hpp"

class AstMain : public AstNode {
 private:
  std::shared_ptr<AstDeclarations> declarations;
  std::shared_ptr<AstCommands> commands;

 public:
  AstMain();
  AstMain(std::shared_ptr<AstDeclarations> declarations, std::shared_ptr<AstCommands> commands);
  ~AstMain() override = default;
  AstMain(const AstMain& from) = default;
  AstMain& operator=(const AstMain& from) = default;
  AstMain(AstMain&&) = default;
  AstMain& operator=(AstMain&&) = default;

  void print() const override;

  std::shared_ptr<AstDeclarations> getDeclarations() const;
  std::shared_ptr<AstCommands> getCommands() const;
};

#endif  // AST_MAIN_HPP
