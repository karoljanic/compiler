#include <iostream>

#include "../../include/ast/AstAssignment.hpp"

AstAssignment::AstAssignment() : AstCommand(CommandType::ASSIGNMENT) {}

AstAssignment::AstAssignment(std::shared_ptr<AstLeftValue> lvalue, std::shared_ptr<AstExpression> expression)
    : AstCommand(CommandType::ASSIGNMENT), lvalue(lvalue), expression(expression) {}

void AstAssignment::print() const {
  std::cout << "AstAssignment {" << std::endl;
  std::cout << "lvalue: ";
  lvalue->print();
  std::cout << "expression: ";
  expression->print();
  std::cout << "}" << std::endl;
}
