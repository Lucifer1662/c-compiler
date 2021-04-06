#include "And.h"
#include "CmpMemory.h"
#include "Regester.h"


And::And(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right)
	: BinaryOperator(std::move(left), std::move(right)) {}


std::tuple<std::string, std::shared_ptr<Memory>> And::operator()(Scope& scope) {
	
	auto instruction = std::string();


	auto addRightInstructions = [&](auto& scope) {
		auto [right_instructions, rightValue] = right->operator()(scope);
		instruction += right_instructions;
	
		return std::move(rightValue);
	};

	auto addLeftInstructions = [&](Scope& scope) {
		auto [left_instructions, leftValue] = left->operator()(scope);
		instruction += left_instructions;
	

		if (leftValue->getType() != MemoryType::CmpMemory) {
			instruction += "cmp " + leftValue->toString() + ",0\n";
			instruction += "je " + scope.useFalseLabel() + "\n";			
		}
		else {
			instruction += ((CmpMemory*)leftValue.get())->getNotComparisonString() + " " + scope.useFalseLabel() + "\n";
		}

		
	};


	auto childScope = scope.createNewLabelScopeWithParentsFalse();

	addLeftInstructions(childScope);

	instruction += childScope.declareTrueLabel();

	auto rightValue = addRightInstructions(childScope);

	return std::make_tuple(instruction, std::move(rightValue));
}
