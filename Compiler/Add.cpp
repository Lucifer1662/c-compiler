#include "Add.h"
#include "Type.h"
#include "Stack.h"
#include "FloatingPoint.h"
#include "FloatingStackManager.h"



Add::Add(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right) : BinaryOperator(std::move(left), std::move(right)) {

}


std::tuple<std::string, std::shared_ptr<Memory>> Add::operator()(Scope& scope) {

	if (left->dataType->id == DataType::Int && right->dataType->id == DataType::Int) {
		return addInts(scope);
	}
	else {
		return addFloats(scope);
	}
}

std::tuple<std::string, std::shared_ptr<Memory>> Add::addInts(Scope& scope)
{
	auto [left_instructions, leftValue] = left->operator()(scope);
	auto instruction = left_instructions;




	leftValue->save();

	auto [right_instructions, rightValue] = right->operator()(scope);

	auto new_right_instructions = leftValue->load(right_instructions);

	instruction += new_right_instructions;


	/*
	we cannot add a mem to a mem

	*/
	if (leftValue->isInMemoryAddress() && rightValue->isInMemoryAddress()) {

		//then we need to convert 1 of these to a regester
		auto [put_instructions, newLeftValue] = leftValue->putInRegester(scope, leftValue);
		instruction += put_instructions;

		instruction += "add " + newLeftValue->toString() + ", " + rightValue->toString() + "\n";
		return std::make_tuple(instruction, std::move(newLeftValue));
	}
	else {
		instruction += "add " + leftValue->toString() + ", " + rightValue->toString() + "\n";

		return std::make_tuple(instruction, std::move(leftValue));
	}


}

std::tuple<std::string, std::shared_ptr<Memory>> Add::addFloats(Scope& scope)
{
	auto [left_instructions, leftValue] = left->operator()(scope);
	auto instruction = left_instructions;


	leftValue->save();

	auto [right_instructions, rightValue] = right->operator()(scope);
	auto new_right_instructions = leftValue->load(right_instructions);
	instruction += new_right_instructions;

	auto [add_instruction, result] = scope.floatingStack->add(scope, leftValue, rightValue);

	instruction += add_instruction;

	return std::make_tuple(instruction, result);
}
