#ifndef AST_ARRAY_HPP
#define AST_ARRAY_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
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

  const std::string& getName() const;
  const std::shared_ptr<AstValue>& getArgument() const;

  virtual void print(std::ostream& out, int tab) const;
};

#endif  // AST_ARRAY_HPP
