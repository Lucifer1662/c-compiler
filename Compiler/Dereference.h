#pragma once
#include "UnaryOperator.h"
struct Dereference : public UnaryOperator {
	std::string operationInt;
	std::string operationFloat;

	Dereference(std::unique_ptr<Expression>&& left);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope, std::shared_ptr<Memory>& value);

};

