#ifndef AST_ARRAY_HPP
#define AST_ARRAY_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "AstLeftValue.hpp"

class AstArray : public AstLeftValue {
 private:
  std::string name;
  std::shared_ptr<AstValue> argument;

 public:
  AstArray();
  AstArray(const std::string& name, std::shared_ptr<AstValue> argument);
  virtual ~AstArray() = default;
  AstArray(const AstArray& from) = default;
  AstArray& operator=(const AstArray& from) = default;
  AstArray(AstArray&&) = default;
  AstArray& operator=(AstArray&&) = default;

  virtual void print() const;
};

#endif  // AST_ARRAY_HPP
