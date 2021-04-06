#include "While.h"
#include "CmpMemory.h"
#include "Expression.h"


While::While(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right) : BinaryOperator(std::move(left), std::move(right)) {}


std::tuple<std::string, std::shared_ptr<Memory>> While::operator()(Scope& scope) {
	auto instruction = std::string();

	auto childScope = scope.createNewLabelScope();

	auto jumpToTop = "jmp " + childScope.useTrueLabel() + "\n";
	instruction += childScope.declareTrueLabel();


	auto [left_instructions, leftValue] = left->operator()(childScope);

	
	
	instruction += left_instructions;


	if (leftValue->getType() != MemoryType::CmpMemory) {
		instruction += "cmp " + leftValue->toString() + ", 0\n";
		instruction += "je " + childScope.useFalseLabel() + "\n";
	}
	else {
		instruction += ((CmpMemory*)leftValue.get())->getNotComparisonString() + " " + childScope.useFalseLabel() + "\n";
	}

	

	auto [right_instructions, rightValue] = right->operator()(scope);



	instruction += right_instructions;
	instruction += jumpToTop;
	instruction += childScope.declareFalseLabel();


	return std::make_tuple(instruction, nullptr);
}