#pragma once
#include "Expression.h"
#include "PrimitiveTypes.h"
#include "Stack.h"

struct Variable : public Expression {
	std::string name;
	int offset=0;


	Variable(std::string name, std::shared_ptr<Type> type);
	Variable(std::string name, std::shared_ptr<Type> type, int offset);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope, std::shared_ptr<Memory>& value);
};

