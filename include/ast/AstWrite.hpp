#ifndef AST_WRITE_HPP
#define AST_WRITE_HPP

#include <memory>

#include "AstCommand.hpp"
#include "AstValue.hpp"

class AstWrite : public AstCommand {
 private:
  std::shared_ptr<AstValue> value;

 public:
  AstWrite();
  AstWrite(std::shared_ptr<AstValue> value);
  virtual ~AstWrite() = default;
  AstWrite(const AstWrite& from) = default;
  AstWrite& operator=(const AstWrite& from) = default;
  AstWrite(AstWrite&&) = default;
  AstWrite& operator=(AstWrite&&) = default;

  virtual void print() const;
};

#endif  // AST_WRITE_HPP