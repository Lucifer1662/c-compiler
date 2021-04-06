#pragma once
#pragma once
#include "BinaryOperator.h"

struct ArithmaticBinary : public BinaryOperator {
	std::string operationInt;
	std::string operationFloat;
	ArithmaticBinary(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right,
		std::string operationInt, std::string operationFloat);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);

	std::tuple<std::string, std::shared_ptr<Memory>> opInts(Scope& scope);
	std::tuple<std::string, std::shared_ptr<Memory>> opFloats(Scope& scope);
};


