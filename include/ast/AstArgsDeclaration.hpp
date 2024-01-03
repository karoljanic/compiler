#ifndef AST_ARGS_DECLARATION_HPP
#define AST_ARGS_DECLARATION_HPP

#include <memory>
#include <ostream>
#include <vector>

#include "AstLeftValue.hpp"
#include "AstNode.hpp"

class AstArgsDeclaration : public AstNode {
 private:
  std::vector<std::shared_ptr<AstLeftValue>> args;

 public:
  AstArgsDeclaration();
  AstArgsDeclaration(std::shared_ptr<AstLeftValue> arg);
  virtual ~AstArgsDeclaration() = default;
  AstArgsDeclaration(const AstArgsDeclaration& from) = default;
  AstArgsDeclaration& operator=(const AstArgsDeclaration& from) = default;
  AstArgsDeclaration(AstArgsDeclaration&&) = default;
  AstArgsDeclaration& operator=(AstArgsDeclaration&&) = default;

  void addArg(std::shared_ptr<AstLeftValue> arg);
  const std::vector<std::shared_ptr<AstLeftValue>>& getArgs() const;

  virtual void print(std::ostream& out, int tab) const override;
};

#endif  // AST_ARGS_DECLARATION_HPP