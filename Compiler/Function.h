#pragma once
#include "Expression.h"
#include <string>
#include "FunctionType.h"

struct Function : public Expression {
	std::string name;
	std::vector<std::unique_ptr<Expression>> inputs;
	std::shared_ptr<FunctionType> functionType;

	Function(std::string name, std::shared_ptr<FunctionType> functionType);
	Function(std::string name, std::vector<std::unique_ptr<Expression>> inputs, std::shared_ptr<FunctionType> functionType);
	

	void addParam(std::unique_ptr<Expression>&& param);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
};



