#pragma once
#include "BinaryOperator.h"
#include <memory>
#include "Variable.h"

struct Assignment : public BinaryOperator {
	Assignment(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);

};

