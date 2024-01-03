#ifndef AST_PROCEDURE_HPP
#define AST_PROCEDURE_HPP

#include <memory>

#include "AstCommands.hpp"
#include "AstDeclarations.hpp"
#include "AstNode.hpp"
#include "AstProcedureHeader.hpp"

class AstProcedure : public AstNode {
 private:
  std::shared_ptr<AstProcedureHeader> header;
  std::shared_ptr<AstDeclarations> declarations;
  std::shared_ptr<AstCommands> commands;

 public:
  AstProcedure();
  AstProcedure(std::shared_ptr<AstProcedureHeader> header, std::shared_ptr<AstDeclarations> declarations,
               std::shared_ptr<AstCommands> commands);
  virtual ~AstProcedure() = default;
  AstProcedure(const AstProcedure& from) = default;
  AstProcedure& operator=(const AstProcedure& from) = default;
  AstProcedure(AstProcedure&&) = default;
  AstProcedure& operator=(AstProcedure&&) = default;

  std::shared_ptr<AstProcedureHeader>& getHeader();
  std::shared_ptr<AstDeclarations>& getDeclarations();
  std::shared_ptr<AstCommands>& getCommands();

  void print() const override;
};

#endif  // AST_PROCEDURE_HPP
