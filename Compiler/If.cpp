#include "If.h"
#include "CmpMemory.h"




If::If(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right) : BinaryOperator(std::move(left), std::move(right)) {}


std::tuple<std::string, std::shared_ptr<Memory>> If::operator()(Scope& scope) {

	auto childScope = scope.createNewLabelScope();
	auto [left_instructions, leftValue] = left->operator()(childScope);
	auto instruction = left_instructions;
	

	if (leftValue->getType() != MemoryType::CmpMemory) {
		instruction += "cmp " + leftValue->toString() + ", 0\n";
		instruction += "je " + childScope.useFalseLabel() + "\n";
	}
	else {
		instruction += ((CmpMemory*)leftValue.get())->getNotComparisonString() + " " + childScope.useFalseLabel() + "\n";
	}
	
	
	
	auto [right_instructions, rightValue] = right->operator()(scope);

	instruction += childScope.declareTrueLabel();

	instruction += right_instructions;

	instruction += childScope.declareFalseLabel();

	return std::make_tuple(instruction, nullptr);
}