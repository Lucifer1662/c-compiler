#pragma once
#include "Expression.h"
#include <memory>

struct UnaryOperator : public Expression {
	std::unique_ptr<Expression> left;
	UnaryOperator(std::unique_ptr<Expression> left) :
		Expression(left->dataType),
		left(std::move(left)) {}

};

