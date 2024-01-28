#ifndef AST_WRITE_HPP
#define AST_WRITE_HPP

#include <memory>
#include <ostream>

#include "AstCommand.hpp"
#include "AstValue.hpp"

class AstWrite : public AstCommand {
 private:
  std::shared_ptr<AstValue> value;

 public:
  AstWrite();
  AstWrite(std::shared_ptr<AstValue> value);
  virtual ~AstWrite() = default;
  AstWrite(const AstWrite &from) = default;
  AstWrite &operator=(const AstWrite &from) = default;
  AstWrite(AstWrite &&) = default;
  AstWrite &operator=(AstWrite &&) = default;

  const std::shared_ptr<AstValue> &getValue() const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  virtual void print(std::ostream &out, int tab) const override;
};

#endif  // AST_WRITE_HPP
