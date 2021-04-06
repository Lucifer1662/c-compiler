#include "IfElse.h"
#include "If.h"
#include "CmpMemory.h"




IfElse::IfElse(std::unique_ptr<Expression>&& condition, std::unique_ptr<Expression>&& ifBody, std::unique_ptr<Expression>&& elseBody) :
	condition(std::move(condition)),
	ifBody(std::move(ifBody)),
	elseBody(std::move(elseBody))

{}


std::tuple<std::string, std::shared_ptr<Memory>> IfElse::operator()(Scope& scope) {

	auto childScope = scope.createNewLabelScope();

	auto [left_instructions, leftValue] = condition->operator()(childScope);
	auto instruction = left_instructions;


	if (leftValue->getType() != MemoryType::CmpMemory) {
		instruction += "cmp " + leftValue->toString() + ", 0\n";
		instruction += "je " + childScope.useFalseLabel() + "\n";
	}
	else {
		instruction += ((CmpMemory*)leftValue.get())->getNotComparisonString() + " " + childScope.useFalseLabel() + "\n";
	}




	auto [ifBody_instructions, ifBodyValue] = ifBody->operator()(scope);

	instruction += ifBody_instructions;


	auto elseLabel = childScope.getUniqueLabel();
	instruction += "jmp " + elseLabel.use() + "\n";

	instruction += childScope.declareFalseLabel();

	auto [else_instructions, elseValue] = elseBody->operator()(scope);
	instruction += else_instructions;

	instruction += elseLabel.declareLabel();

	return std::make_tuple(instruction, nullptr);
}