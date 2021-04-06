#pragma once
#include "UnaryOperator.h"
#include <memory>

struct Return : public UnaryOperator {
	Return(std::unique_ptr<Expression>&& left);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);


};


