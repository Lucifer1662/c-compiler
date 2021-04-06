#pragma once
#include "Expression.h"
#include <memory>

struct ForLoop : public Expression
{
	std::unique_ptr<Expression> initiator;
	std::unique_ptr<Expression> comparator;
	std::unique_ptr<Expression> incrementer;
	std::unique_ptr<Expression> body;

	ForLoop(std::unique_ptr<Expression>&& initiator, std::unique_ptr<Expression>&& comparator,
		std::unique_ptr<Expression>&& incrementer, std::unique_ptr<Expression>&& body);
	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
};

