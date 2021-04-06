#include "Assignment.h"

Assignment::Assignment(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r) : BinaryOperator(std::move(l), std::move(r)) {}



std::tuple<std::string, std::shared_ptr<Memory>> Assignment::operator()(Scope& scope) {

	auto [right_instructions, regR] = right->operator()(scope);


	auto instructions = right_instructions;



	regR->save();

	auto [left_instructions, leftValue] = left->operator()(scope);

	auto new_left_instructions = regR->load(left_instructions);

	instructions += new_left_instructions;


	if (regR->isInMemoryAddress()) {
		auto [put_instructions, newValuePut] = regR->putInRegester(scope, regR);
		instructions += put_instructions;
		regR = newValuePut;
	}

	instructions += "mov " + leftValue->toString() + ", " + regR->toString() + "\n";

	return std::make_tuple(instructions, std::move(leftValue));
}