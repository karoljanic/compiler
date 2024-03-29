#include "../../include/ast/AstCondition.hpp"

AstCondition::AstCondition() : AstNode(NodeType::CONDITION) {}

AstCondition::AstCondition(ConditionType conditionType, std::shared_ptr<AstValue> left, std::shared_ptr<AstValue> right)
	: AstNode(NodeType::CONDITION), conditionType(conditionType), left(left), right(right) {}

AstCondition::ConditionType AstCondition::getConditionType() const {
  return conditionType;
}

const std::shared_ptr<AstValue> &AstCondition::getLeft() const {
  return left;
}

const std::shared_ptr<AstValue> &AstCondition::getRight() const {
  return right;
}

std::shared_ptr<AstNode> AstCondition::copy(const std::map<std::string, std::string> &rewriteTable) const {
  return std::make_shared<AstCondition>(conditionType,
										std::dynamic_pointer_cast<AstValue>(left->copy(rewriteTable)),
										std::dynamic_pointer_cast<AstValue>(right->copy(rewriteTable)));
}

void AstCondition::print(std::ostream &out, int tab) const {
  left->print(out, tab);
  switch (conditionType) {
	case EQUAL:out << " == ";
	  break;
	case NOT_EQUAL:out << " != ";
	  break;
	case LESS:out << " < ";
	  break;
	case GREATER:out << " > ";
	  break;
	case LESS_EQUAL:out << " <= ";
	  break;
	case GREATER_EQUAL:out << " >= ";
	  break;
  }
  right->print(out, 0);
}
