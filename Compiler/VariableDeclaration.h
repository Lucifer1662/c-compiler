#pragma once
#include "Variable.h"

struct VariableDeclaration : public Variable {

	VariableDeclaration(std::string name, std::shared_ptr<Type> dataType) : Variable(name, dataType) {}

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope) {

		scope.addName(name);
		//currentScope->variables.push_back(name);


		//get the regest the right hand side is in
		auto instructions = "sub esp, byte " + std::to_string(dataType->size) + "\n";
	

		return std::make_tuple(instructions, std::make_shared<Stack>(scope.getStackPtrOffsetForVar(name), dataType));
	}

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope, std::shared_ptr<Memory>& value) {
		value->dataType = dataType;

		///currentScope->variables.push_back(name);
		auto instructions = std::string();


		auto [put_instructions, newValue] = value->putAtTopOfStack(scope, name, value);
		instructions += put_instructions;
		value = newValue;
		return { instructions, value };
	}
};

