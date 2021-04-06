#include "ArithmaticUnary.h"
#include "Type.h"
#include "Stack.h"
#include "FloatingPoint.h"
#include "FloatingStackManager.h"



ArithmaticUnary::ArithmaticUnary(std::unique_ptr<Expression>&& left,
	std::string operationInt, std::string operationFloat)
	: UnaryOperator(std::move(left)), operationInt(operationInt), operationFloat(operationFloat) {

}


std::tuple<std::string, std::shared_ptr<Memory>> ArithmaticUnary::operator()(Scope& scope) {

	if (left->dataType->id == DataType::Int) {
		return opInts(scope);
	}
	else {
		return opFloats(scope);
	}
}

std::tuple<std::string, std::shared_ptr<Memory>> ArithmaticUnary::opInts(Scope& scope)
{
	auto [left_instructions, leftValue] = left->operator()(scope);
	auto instruction = left_instructions;


	instruction += operationInt + " " + leftValue->getSizeName() + " " + leftValue->toString() + "\n";

	return std::make_tuple(instruction, std::move(leftValue));
}

std::tuple<std::string, std::shared_ptr<Memory>> ArithmaticUnary::opFloats(Scope& scope)
{
	auto [left_instructions, leftValue] = left->operator()(scope);
	auto instruction = left_instructions;

	auto [add_instruction, result] = scope.floatingStack->operation(scope, leftValue, operationFloat);

	instruction += add_instruction;

	return std::make_tuple(instruction, result);
}
