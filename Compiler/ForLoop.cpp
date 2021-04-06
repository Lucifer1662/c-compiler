#include "ForLoop.h"
#include "CmpMemory.h"
#include "Expression.h"


ForLoop::ForLoop(std::unique_ptr<Expression>&& initiator, std::unique_ptr<Expression>&& comparator,
	std::unique_ptr<Expression>&& incrementer, std::unique_ptr<Expression>&& body) :
	initiator(std::move(initiator)),
	comparator(std::move(comparator)),
	incrementer(std::move(incrementer)),
	body(std::move(body))
{}

std::tuple<std::string, std::shared_ptr<Memory>> ForLoop::operator()(Scope& scope) {
	auto instruction = std::string();


	auto childScope = scope.createNewLabelScope();

	auto [init_instructions, initValue] = initiator->operator()(childScope);
	instruction += init_instructions;

	

	auto jumpToTop = "jmp " + childScope.useTrueLabel() + "\n";
	instruction += childScope.declareTrueLabel();

	auto [comparator_instructions, comparatorValue] = comparator->operator()(childScope);
	instruction += comparator_instructions;

	if (comparatorValue->getType() != MemoryType::CmpMemory) {
		instruction += "cmp " + comparatorValue->toString() + ", 0\n";
		instruction += "je " + childScope.useFalseLabel() + "\n";
	}
	else {
		instruction += ((CmpMemory*)comparatorValue.get())->getNotComparisonString() + " " + childScope.useFalseLabel() + "\n";
	}


	auto [body_instructions, bodyValue] = body->operator()(childScope);
	instruction += body_instructions;

	auto [incrementer_instructions, incrementerValue] = incrementer->operator()(childScope);
	instruction += incrementer_instructions;

	instruction += jumpToTop;

	instruction += childScope.declareFalseLabel();


	return std::make_tuple(instruction, nullptr);
}