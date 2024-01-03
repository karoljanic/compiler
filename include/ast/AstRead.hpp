#ifndef AST_READ_HPP
#define AST_READ_HPP

#include <memory>

#include "AstCommand.hpp"
#include "AstLeftValue.hpp"

class AstRead : public AstCommand {
 private:
  std::shared_ptr<AstLeftValue> lvalue;

 public:
  AstRead();
  AstRead(std::shared_ptr<AstLeftValue> lvalue);
  virtual ~AstRead() = default;
  AstRead(const AstRead& from) = default;
  AstRead& operator=(const AstRead& from) = default;
  AstRead(AstRead&&) = default;
  AstRead& operator=(AstRead&&) = default;

  virtual void print() const;
};

#endif  // AST_READ_HPP