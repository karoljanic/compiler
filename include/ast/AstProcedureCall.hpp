#ifndef AST_PROCEDURE_CALL_HPP
#define AST_PROCEDURE_CALL_HPP

#include <memory>
#include <ostream>
#include <string>
#include <map>

#include "AstArgsList.hpp"
#include "AstCommand.hpp"

class AstProcedureCall : public AstCommand {
 private:
  std::string name;
  std::shared_ptr<AstArgsList> argsList;

  static std::map<std::string, size_t> functionCallsCounter;

 public:
  AstProcedureCall();
  AstProcedureCall(std::string name, std::shared_ptr<AstArgsList> argsList);
  virtual ~AstProcedureCall() = default;
  AstProcedureCall(const AstProcedureCall &from) = default;
  AstProcedureCall &operator=(const AstProcedureCall &from) = default;
  AstProcedureCall(AstProcedureCall &&) = default;
  AstProcedureCall &operator=(AstProcedureCall &&) = default;

  const std::string &getName() const;
  const std::shared_ptr<AstArgsList> &getArgsList() const;

  virtual std::shared_ptr<AstNode> copy(const std::map<std::string, std::string> &rewriteTable) const override;
  virtual void print(std::ostream &out, int tab) const override;

  static const std::map<std::string, size_t> &getFunctionCallsCounter();
};

#endif  // AST_PROCEDURE_CALL_HPP
