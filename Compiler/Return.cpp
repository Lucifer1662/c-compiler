#include "Return.h"

Return::Return(std::unique_ptr<Expression>&& left) : UnaryOperator(std::move(left)) {}

std::tuple<std::string, std::shared_ptr<Memory>> Return::operator()(Scope& scope) {
	auto [instructions, value] = left->operator()(scope);


	//put in reg eax
	auto [moveToEax, eax] = value->putInRegester(scope, 0);
	moveToEax +=  "pop ebp \nret \n";

	return std::make_tuple(instructions + moveToEax, std::move(eax));
}
