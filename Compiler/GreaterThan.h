#pragma once
#include "BinaryOperator.h"
#include <memory>

struct GreaterThan : public BinaryOperator
{
	GreaterThan(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right);
	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
};

