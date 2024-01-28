#ifndef AST_PROCEDURE_HPP
#define AST_PROCEDURE_HPP

#include <memory>
#include <ostream>

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
  AstProcedure(const AstProcedure &from) = default;
  AstProcedure &operator=(const AstProcedure &from) = default;
  AstProcedure(AstProcedure &&) = default;
  AstProcedure &operator=(AstProcedure &&) = default;

  void setDeclarations(std::shared_ptr<AstDeclarations> declarations);
  void setCommands(std::shared_ptr<AstCommands> commands);

  const std::shared_ptr<AstProcedureHeader> &getHeader();
  const std::shared_ptr<AstDeclarations> &getDeclarations();
  const std::shared_ptr<AstCommands> &getCommands();

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  void print(std::ostream &out, int tab) const override;
};

#endif  // AST_PROCEDURE_HPP
