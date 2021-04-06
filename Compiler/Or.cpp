#include "Or.h"
#include "And.h"
#include "CmpMemory.h"
#include "Regester.h" 


Or::Or(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right)
	: BinaryOperator(std::move(left), std::move(right)) {}


std::tuple<std::string, std::shared_ptr<Memory>> Or::operator()(Scope& scope) {

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
			instruction += "cmp " + leftValue->toString() + ",0\n";\
			instruction += std::string("jne ") + scope.useTrueLabel() + '\n';
		}
		else {
			instruction += ((CmpMemory*)leftValue.get())->getComparisonString() + " " + scope.useTrueLabel() + "\n";
		}

		
	};

		auto childScope = scope.createNewLabelScopeWithParentsTrue();

		addLeftInstructions(childScope);

		instruction += childScope.declareFalseLabel();

		auto rightValue = addRightInstructions(childScope);





		return std::make_tuple(instruction, std::move(rightValue));
	

}
