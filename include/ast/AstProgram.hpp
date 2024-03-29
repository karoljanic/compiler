#ifndef AST_PROGRAM_HPP
#define AST_PROGRAM_HPP

#include <memory>
#include <ostream>

#include "AstMain.hpp"
#include "AstNode.hpp"
#include "AstProcedures.hpp"

class AstProgram : public AstNode {
 private:
  std::shared_ptr<AstProcedures> procedures;
  std::shared_ptr<AstMain> main;

 public:
  AstProgram();
  AstProgram(std::shared_ptr<AstProcedures> procedures, std::shared_ptr<AstMain> main);
  ~AstProgram() override = default;
  AstProgram(const AstProgram &from) = default;
  AstProgram &operator=(const AstProgram &from) = default;
  AstProgram(AstProgram &&) = default;
  AstProgram &operator=(AstProgram &&) = default;

  void setMain(std::shared_ptr<AstMain> main);
  void setProcedures(std::shared_ptr<AstProcedures> procedures);

  const std::shared_ptr<AstProcedures> &getProcedures() const;
  const std::shared_ptr<AstMain> &getMain() const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  void print(std::ostream &out, int tab) const override;
};

#endif  // AST_PROGRAM_HPP
