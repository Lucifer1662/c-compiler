#include "GreaterThan.h"
#include "CmpMemory.h"

GreaterThan::GreaterThan(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right)
	: BinaryOperator(std::move(left), std::move(right)) {}


std::tuple<std::string, std::shared_ptr<Memory>> GreaterThan::operator()(Scope& scope) {
	auto [left_instructions, leftValue] = left->operator()(scope);
	auto instruction = left_instructions;

	leftValue->save();

	auto [right_instructions, rightValue] = right->operator()(scope);

	auto new_right_instructions = leftValue->load(right_instructions);

	instruction += new_right_instructions;

	/*
	we cannot add a mem to a mem
	*/
	if ((leftValue->getType() == MemoryType::Stack || leftValue->getType() == MemoryType::Heap) &&
		(rightValue->getType() == MemoryType::Stack || rightValue->getType() == MemoryType::Heap)) {

		//then we need to convert 1 of these to a regester
		auto [put_instructions, newLeftValue] = leftValue->putInRegester(scope, leftValue);
		instruction += put_instructions;

		instruction += "cmp " + newLeftValue->toString() + ", " + rightValue->toString() + "\n";
	}
	else {
		instruction += "cmp " + leftValue->toString() + ", " + rightValue->toString() + "\n";
	}

	return std::make_tuple(instruction, std::make_unique<CmpMemory>(CmpMemory::ComparisonEnum::GreaterThan, dataType));
}
