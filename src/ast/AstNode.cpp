#include <iostream>

#include "../../include/ast/AstNode.hpp"

AstNode::AstNode() : nodeType(NodeType::DEFAULT) {}

AstNode::AstNode(NodeType type) : nodeType(type) {}

AstNode::NodeType AstNode::getNodeType() const {
  return nodeType;
}

void AstNode::print() const {
  std::cout << "AstNode" << std::endl;
}
