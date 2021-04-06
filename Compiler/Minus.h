#pragma once
#include "BinaryOperator.h"

struct Minus : public BinaryOperator {
	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
};

