#ifndef AST_DECLARATIONS_HPP
#define AST_DECLARATIONS_HPP

#include <memory>
#include <ostream>
#include <vector>

#include "AstLeftValue.hpp"
#include "AstNode.hpp"

class AstDeclarations : public AstNode {
 private:
  std::vector<std::shared_ptr<AstLeftValue>> declarations;

 public:
  AstDeclarations();
  AstDeclarations(std::shared_ptr<AstLeftValue> declaration);
  ~AstDeclarations() override = default;
  AstDeclarations(const AstDeclarations& from) = default;
  AstDeclarations& operator=(const AstDeclarations& from) = default;
  AstDeclarations(AstDeclarations&&) = default;
  AstDeclarations& operator=(AstDeclarations&&) = default;

  void addDeclaration(std::shared_ptr<AstLeftValue> declaration);
  const std::vector<std::shared_ptr<AstLeftValue>>& getDeclarations() const;

  void print(std::ostream& out, int tab) const override;
};

#endif  // AST_DECLARATIONS_HPP
