#pragma once
#include "BinaryOperator.h"
struct IfElse : Expression
{
	std::unique_ptr<Expression> condition;
	std::unique_ptr<Expression> ifBody;
	std::unique_ptr<Expression> elseBody;

	IfElse(std::unique_ptr<Expression>&& condition, std::unique_ptr<Expression>&& ifBody, std::unique_ptr<Expression>&& elseBody);
	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
};

