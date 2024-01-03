#ifndef AST_NUMBER_HPP
#define AST_NUMBER_HPP

#include <cstdint>
#include <ostream>

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

  uint64_t getValue() const;

  virtual void print(std::ostream& out, int tab) const;
};

#endif  // AST_NUMBER_HPP
