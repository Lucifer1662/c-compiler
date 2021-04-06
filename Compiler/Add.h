#pragma once
#include "BinaryOperator.h"

struct Add : public BinaryOperator {

	Add(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);

	std::tuple<std::string, std::shared_ptr<Memory>> addInts(Scope& scope);
	std::tuple<std::string, std::shared_ptr<Memory>> addFloats(Scope& scope);
};


