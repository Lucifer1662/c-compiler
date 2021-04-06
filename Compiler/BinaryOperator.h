#pragma once
#include "Expression.h"

struct BinaryOperator : public Expression {

	BinaryOperator(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);

	std::unique_ptr<Expression> left;
	std::unique_ptr<Expression> right;
};

