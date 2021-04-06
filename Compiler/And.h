#pragma once
#include <memory>
#include "Expression.h"
#include "BinaryOperator.h"

struct And : public BinaryOperator
{
	And(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
};

