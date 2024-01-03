#ifndef AST_NUMBER_HPP
#define AST_NUMBER_HPP

#include <cstdint>
#include <iostream>

#include "AstRightValue.hpp"

class AstNumber : public AstRightValue {
 private:
  uint64_t value;

 public:
  AstNumber();
  AstNumber(uint64_t val);
  virtual ~AstNumber() = default;
  AstNumber(const AstNumber& from) = default;
  AstNumber& operator=(const AstNumber& from) = default;
  AstNumber(AstNumber&&) = default;
  AstNumber& operator=(AstNumber&&) = default;

  virtual void print() const;
};

#endif  // AST_NUMBER_HPP
