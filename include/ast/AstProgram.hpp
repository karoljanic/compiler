#ifndef AST_PROGRAM_HPP
#define AST_PROGRAM_HPP

#include <memory>

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
  AstProgram(const AstProgram& from) = default;
  AstProgram& operator=(const AstProgram& from) = default;
  AstProgram(AstProgram&&) = default;
  AstProgram& operator=(AstProgram&&) = default;

  void print() const override;

  std::shared_ptr<AstProcedures> getProcedures() const;
  std::shared_ptr<AstMain> getMain() const;
};

#endif  // AST_PROGRAM_HPP
