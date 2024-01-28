#ifndef AST_ARGS_LIST_HPP
#define AST_ARGS_LIST_HPP

#include <memory>
#include <ostream>
#include <vector>

#include "AstLeftValue.hpp"
#include "AstArray.hpp"
#include "AstNode.hpp"

class AstArgsList : public AstNode {
 private:
  std::vector<std::shared_ptr<AstLeftValue>> args;

 public:
  AstArgsList();
  AstArgsList(std::shared_ptr<AstLeftValue> arg);
  virtual ~AstArgsList() = default;
  AstArgsList(const AstArgsList &from) = default;
  AstArgsList &operator=(const AstArgsList &from) = default;
  AstArgsList(AstArgsList &&) = default;
  AstArgsList &operator=(AstArgsList &&) = default;

  void addArg(std::shared_ptr<AstLeftValue> arg);
  const std::vector<std::shared_ptr<AstLeftValue>> &getArgs() const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  virtual void print(std::ostream &out, int tab) const override;
};

#endif  // AST_ARGS_LIST_HPP
