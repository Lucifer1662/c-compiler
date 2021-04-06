#pragma once
#include <vector>
#include "Expression.h"
#include <memory>

struct Execution : public Expression {
	std::vector<std::unique_ptr<Expression>> statements;
	virtual std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
	virtual std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope, std::shared_ptr<Memory>& reg);
	void addStatement(std::unique_ptr<Expression> expression);
};

