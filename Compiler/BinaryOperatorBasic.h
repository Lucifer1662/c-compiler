#pragma once
#include <memory>
#include "Expression.h"
#include "BinaryOperator.h"

template<const char* operation>
class BinaryOperatorBasic : public BinaryOperator
{
	BinaryOperatorBasic(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right)
		: BinaryOperator(std::move(left), std::move(right)) {}


	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope) {

		auto [left_instructions, leftValue] = left->operator()(scope);
		auto instruction = left_instructions;




		leftValue->save();

		auto [right_instructions, rightValue] = right->operator()(scope);

		auto new_right_instructions = leftValue->load(right_instructions);

		instruction += new_right_instructions;

		if ((leftValue->getType() == MemoryType::Stack || leftValue->getType() == MemoryType::Heap) &&
			(rightValue->getType() == MemoryType::Stack || rightValue->getType() == MemoryType::Heap)) {

			//then we need to convert 1 of these to a regester
			auto [put_instructions, newLeftValue] = leftValue->putInRegester();
			instruction += put_instructions;

			instruction += std::string(operation) + " " + newLeftValue->toString() + ", " + rightValue->toString() + "\n";
			return std::make_tuple(instruction, std::move(newLeftValue));
		}
		else {
			instruction += std::string(operation) + " " + leftValue->toString() + ", " + rightValue->toString() + "\n";

			return std::make_tuple(instruction, std::move(leftValue));
		}




	}
};

