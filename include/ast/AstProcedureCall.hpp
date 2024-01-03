#ifndef AST_PROCEDURE_CALL_HPP
#define AST_PROCEDURE_CALL_HPP

#include <memory>
#include <string>

#include "AstArgsList.hpp"
#include "AstCommand.hpp"

class AstProcedureCall : public AstCommand {
 private:
  std::string name;
  std::shared_ptr<AstArgsList> argsList;

 public:
  AstProcedureCall();
  AstProcedureCall(std::string name, std::shared_ptr<AstArgsList> argsList);
  virtual ~AstProcedureCall() = default;
  AstProcedureCall(const AstProcedureCall& from) = default;
  AstProcedureCall& operator=(const AstProcedureCall& from) = default;
  AstProcedureCall(AstProcedureCall&&) = default;
  AstProcedureCall& operator=(AstProcedureCall&&) = default;

  virtual void print() const;
};

#endif  // AST_PROCEDURE_CALL_HPP
