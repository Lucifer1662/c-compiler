#pragma once
#include <memory>
#include "Expression.h"
#include "BinaryOperator.h"

struct Or : public BinaryOperator
{
	Or(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
};

