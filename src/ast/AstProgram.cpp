#include <iostream>

#include "../../include/ast/AstProgram.hpp"

AstProgram::AstProgram() : AstNode(NodeType::PROGRAM) {}

AstProgram::AstProgram(std::shared_ptr<AstProcedures> procedures, std::shared_ptr<AstMain> main)
    : AstNode(NodeType::PROGRAM), procedures(procedures), main(main) {}

void AstProgram::print() const {
  std::cout << "Program" << std::endl;
  procedures->print();
  main->print();
}
