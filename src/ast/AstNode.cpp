#include "../../include/ast/AstNode.hpp"

AstNode::AstNode() : nodeType(NodeType::DEFAULT) {}

AstNode::AstNode(NodeType type) : nodeType(type) {}

AstNode::NodeType AstNode::getNodeType() const {
  return nodeType;
}

void AstNode::print(std::ostream& out, int tab) const {
  out << std::string(tab, ' ') << "NODE";
}
