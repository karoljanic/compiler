#include "../../include/ast/AstProgram.hpp"

AstProgram::AstProgram() : AstNode(NodeType::PROGRAM) {}

AstProgram::AstProgram(std::shared_ptr<AstProcedures> procedures, std::shared_ptr<AstMain> main)
	: AstNode(NodeType::PROGRAM), procedures(procedures), main(main) {}

void AstProgram::setMain(std::shared_ptr<AstMain> newMain) {
  main = newMain;
}

void AstProgram::setProcedures(std::shared_ptr<AstProcedures> newProcedures) {
  procedures = newProcedures;
}

const std::shared_ptr<AstProcedures> &AstProgram::getProcedures() const {
  return procedures;
}

const std::shared_ptr<AstMain> &AstProgram::getMain() const {
  return main;
}

std::shared_ptr<AstNode> AstProgram::copy(const std::map<std::string, std::string> &rewriteTable) const {
  return std::make_shared<AstProgram>(std::dynamic_pointer_cast<AstProcedures>(procedures->copy(rewriteTable)),
									  std::dynamic_pointer_cast<AstMain>(main->copy(rewriteTable)));
}

void AstProgram::print(std::ostream &out, int tab) const {
  out << std::string(tab, ' ') << "PROGRAM" << std::endl;
  procedures->print(out, tab + 4);
  main->print(out, tab + 4);
  out << std::string(tab, ' ') << "ENDPROGRAM" << std::endl;
}
