#ifndef AST_PROCEDURES_HPP
#define AST_PROCEDURES_HPP

#include <memory>
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
  AstProcedures(const AstProcedures& from) = default;
  AstProcedures& operator=(const AstProcedures& from) = default;
  AstProcedures(AstProcedures&&) = default;
  AstProcedures& operator=(AstProcedures&&) = default;

  void addProcedure(std::shared_ptr<AstProcedure> procedure);

  std::vector<std::shared_ptr<AstProcedure>>& getProcedures();
  void print() const override;
};

#endif  // AST_PROCEDURES_HPP