#pragma once
#include "BinaryOperator.h"
struct If : BinaryOperator
{
	If(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right);
	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
};

