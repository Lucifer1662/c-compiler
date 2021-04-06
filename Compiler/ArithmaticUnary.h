#pragma once
#include "UnaryOperator.h"
struct ArithmaticUnary : public UnaryOperator {
	std::string operationInt;
	std::string operationFloat;
	ArithmaticUnary(std::unique_ptr<Expression>&& left,
		std::string operationInt, std::string operationFloat);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);

	std::tuple<std::string, std::shared_ptr<Memory>> opInts(Scope& scope);
	std::tuple<std::string, std::shared_ptr<Memory>> opFloats(Scope& scope);
};


