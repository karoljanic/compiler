#ifndef AST_PROCEDURES_HPP
#define AST_PROCEDURES_HPP

#include <memory>
#include <ostream>
#include <vector>

#include "AstNode.hpp"
#include "AstProcedure.hpp"

class AstProcedures : public AstNode {
 private:
  std::vector<std::shared_ptr<AstProcedure>> procedures;

 public:
  AstProcedures();
  AstProcedures(std::shared_ptr<AstProcedure> procedure);
  virtual ~AstProcedures() = default;
  AstProcedures(const AstProcedures &from) = default;
  AstProcedures &operator=(const AstProcedures &from) = default;
  AstProcedures(AstProcedures &&) = default;
  AstProcedures &operator=(AstProcedures &&) = default;

  void addProcedure(std::shared_ptr<AstProcedure> procedure);
  const std::vector<std::shared_ptr<AstProcedure>> &getProcedures() const;
  const std::shared_ptr<AstProcedure> &getProcedure(const std::string &name) const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  void print(std::ostream &out, int tab) const override;
};

#endif  // AST_PROCEDURES_HPP
