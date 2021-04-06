#pragma once
#include "Execution.h"
#include "FunctionType.h"

struct FunctionDeclaration : public Execution {
	std::string name;
	std::vector<std::string> inputs;
	std::shared_ptr<FunctionType> functionType;

	FunctionDeclaration(std::string name, std::vector<std::string> inputs, std::shared_ptr<FunctionType> functionType)
		: name(name), inputs(inputs), functionType(functionType) {}

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope) {


		for (int i = inputs.size() - 1; i >= 0; i--)
			scope.addName(inputs[i], functionType->parameters[i]->size);


		scope.addName(name, 4);

		auto myScope = scope.childScope();


		//get the regest the right hand side is in
		auto instructions = "_" + name + ":\n";
		if (name == "main") {
			scope.addToGlobal(std::string("__floatControl"), "__floatControl dw 0\n");
			instructions += "fninit \n";
			instructions += "fnstcw[__floatControl]\n";
			instructions += "mov ax, [__floatControl]\n";
			instructions += "or ax, 3072\n";
			instructions += "mov[__floatControl], ax\n";
			instructions += "fldcw[__floatControl]\n";
		}

		instructions += "push ebp\n";
		instructions += "mov ebp, esp\n";
		scope.addUnnamed(4);

		//insert function code here
		auto [body_instructions, _] = Execution::operator()(myScope);
		instructions += body_instructions;

		instructions += "pop ebp\n";
		instructions += "ret \n";



		return std::make_tuple(instructions, nullptr);
	}
};


