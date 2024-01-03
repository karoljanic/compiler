#ifndef AST_PROCEDURE_HEADER_HPP
#define AST_PROCEDURE_HEADER_HPP

#include <memory>
#include <string>

#include "AstArgsDeclaration.hpp"
#include "AstNode.hpp"

class AstProcedureHeader : public AstNode {
 private:
  std::string name;
  std::shared_ptr<AstArgsDeclaration> argsDeclaration;

 public:
  AstProcedureHeader();
  AstProcedureHeader(const std::string& name, std::shared_ptr<AstArgsDeclaration> argsDeclaration);
  virtual ~AstProcedureHeader() = default;
  AstProcedureHeader(const AstProcedureHeader& from) = default;
  AstProcedureHeader& operator=(const AstProcedureHeader& from) = default;
  AstProcedureHeader(AstProcedureHeader&&) = default;
  AstProcedureHeader& operator=(AstProcedureHeader&&) = default;

  virtual void print() const;
};

#endif  // AST_PROCEDURE_HEADER_HPP
