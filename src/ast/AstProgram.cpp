#include "../../include/ast/AstProgram.hpp"

AstProgram::AstProgram() : AstNode(NodeType::PROGRAM) {}

AstProgram::AstProgram(std::shared_ptr<AstProcedures> procedures, std::shared_ptr<AstMain> main)
    : AstNode(NodeType::PROGRAM), procedures(procedures), main(main) {}

const std::shared_ptr<AstProcedures>& AstProgram::getProcedures() const {
  return procedures;
}

const std::shared_ptr<AstMain>& AstProgram::getMain() const {
  return main;
}

void AstProgram::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "PROGRAM" << std::endl;
  procedures->print(out, tab + 4);
  main->print(out, tab + 4);
  out << std::string(tab, ' ') << "ENDPROGRAM" << std::endl;
}
