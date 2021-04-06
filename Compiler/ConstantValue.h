#pragma once
#include "Expression.h"
#include "Regester.h"
#include <tuple>

struct ConstantValue : public Expression {

	ConstantValue(std::string value, std::shared_ptr<Type> dataType);
	std::string value;

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
};

