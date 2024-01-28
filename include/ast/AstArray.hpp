#ifndef AST_ARRAY_HPP
#define AST_ARRAY_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>

#include "AstLeftValue.hpp"
#include "AstVariable.hpp"
#include "AstNumber.hpp"

class AstArray : public AstLeftValue {
 private:
  std::shared_ptr<AstValue> argument;

 public:
  AstArray();
  AstArray(const std::string &name, std::shared_ptr<AstValue> argument);
  virtual ~AstArray() = default;
  AstArray(const AstArray &from) = default;
  AstArray &operator=(const AstArray &from) = default;
  AstArray(AstArray &&) = default;
  AstArray &operator=(AstArray &&) = default;

  const std::string &getName() const;
  const std::shared_ptr<AstValue> &getArgument() const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  virtual void print(std::ostream &out, int tab) const override;
};

#endif  // AST_ARRAY_HPP
