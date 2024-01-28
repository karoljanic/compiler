#ifndef AST_READ_HPP
#define AST_READ_HPP

#include <memory>
#include <ostream>

#include "AstCommand.hpp"
#include "AstLeftValue.hpp"

class AstRead : public AstCommand {
 private:
  std::shared_ptr<AstLeftValue> lvalue;

 public:
  AstRead();
  AstRead(std::shared_ptr<AstLeftValue> lvalue);
  virtual ~AstRead() = default;
  AstRead(const AstRead &from) = default;
  AstRead &operator=(const AstRead &from) = default;
  AstRead(AstRead &&) = default;
  AstRead &operator=(AstRead &&) = default;

  const std::shared_ptr<AstLeftValue> &getLvalue() const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  virtual void print(std::ostream &out, int tab) const override;
};

#endif  // AST_READ_HPP
