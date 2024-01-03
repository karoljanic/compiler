#ifndef AST_ARGS_LIST_HPP
#define AST_ARGS_LIST_HPP

#include <memory>
#include <vector>

#include "AstLeftValue.hpp"
#include "AstNode.hpp"

class AstArgsList : public AstNode {
 private:
  std::vector<std::shared_ptr<AstLeftValue>> args;

 public:
  AstArgsList();
  AstArgsList(std::shared_ptr<AstLeftValue> arg);
  virtual ~AstArgsList() = default;
  AstArgsList(const AstArgsList& from) = default;
  AstArgsList& operator=(const AstArgsList& from) = default;
  AstArgsList(AstArgsList&&) = default;
  AstArgsList& operator=(AstArgsList&&) = default;

  void addArg(std::shared_ptr<AstLeftValue> arg);

  virtual void print() const override;
};

#endif  // AST_ARGS_LIST_HPP
