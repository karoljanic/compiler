#ifndef AST_LEFT_VALUE_HPP
#define AST_LEFT_VALUE_HPP

#include <ostream>
#include <string>

#include "AstValue.hpp"

class AstLeftValue : public AstValue {
 protected:
  std::string name;
  bool initialized;

 public:
  AstLeftValue();
  AstLeftValue(NodeType type, std::string name, bool initialized);
  virtual ~AstLeftValue() = default;
  AstLeftValue(const AstLeftValue &from) = default;
  AstLeftValue &operator=(const AstLeftValue &from) = default;
  AstLeftValue(AstLeftValue &&) = default;
  AstLeftValue &operator=(AstLeftValue &&) = default;

  void initialize();
  bool isInitialized() const;
  const std::string &getName() const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  virtual void print(std::ostream &out, int tab) const override;
};

#endif  // AST_LEFT_VALUE_HPP
