#include "Variable.h"

Variable::Variable(std::string name, std::shared_ptr<Type> dataType) : name(name), Expression(dataType){}
Variable::Variable(std::string name, std::shared_ptr<Type> dataType, int offset) : name(name), Expression(dataType), offset(offset) {}


std::tuple<std::string, std::shared_ptr<Memory>> Variable::operator()(Scope& scope) {

	auto reg = std::make_unique<Stack>(scope.getStackPtrOffsetForVar(name)-offset, dataType);

	//get the regest the right hand side is in
	//auto mov = "mov " + reg->toString() + ", " + + " \n";

	return std::make_tuple("", std::move(reg));
}


std::tuple<std::string, std::shared_ptr<Memory>> Variable::operator()
(Scope& scope, std::shared_ptr<Memory>& value) {
	auto newValue = value;

	///currentScope->variables.push_back(name);
	auto instructions = std::string();
	auto stack = std::make_shared<Stack>(scope.getStackPtrOffsetForVar(name) - offset, dataType);
	if (value->isInMemoryAddress()) {
		auto [put_instructions, newValuePut] = value->putInRegester(scope, value);
		instructions += put_instructions;
		newValue = newValuePut;
	}

	instructions += "mov " + stack->toString() + ", " + newValue->toString() + "\n";
	return { instructions, value };
}