#include "../../include/ast/AstAssignment.hpp"

AstAssignment::AstAssignment() : AstCommand(CommandType::ASSIGNMENT) {}

AstAssignment::AstAssignment(std::shared_ptr<AstLeftValue> lvalue, std::shared_ptr<AstExpression> expression)
    : AstCommand(CommandType::ASSIGNMENT), lvalue(lvalue), expression(expression) {}

const std::shared_ptr<AstLeftValue>& AstAssignment::getLvalue() const {
  return lvalue;
}

const std::shared_ptr<AstExpression>& AstAssignment::getExpression() const {
  return expression;
}

void AstAssignment::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "ASSIGN ";
  lvalue->print(out, 0);
  out << " := ";
  expression->print(out, 0);
}
