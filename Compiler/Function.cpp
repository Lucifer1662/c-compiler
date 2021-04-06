#include "Function.h"
#include "Regester.h"
#include "Stack.h"
#include "FloatingPoint.h"

template <typename T>
struct reversion_wrapper { T& iterable; };

template <typename T>
auto begin(reversion_wrapper<T> w) { return std::rbegin(w.iterable); }

template <typename T>
auto end(reversion_wrapper<T> w) { return std::rend(w.iterable); }

template <typename T>
reversion_wrapper<T> reverse(T&& iterable) { return { iterable }; }

Function::Function(std::string name, std::vector<std::unique_ptr<Expression>> inputs, std::shared_ptr<FunctionType> functionType)
	: name(name), inputs(std::move(inputs)), Expression(functionType->returnType), functionType(functionType) {}

Function::Function(std::string name, std::shared_ptr<FunctionType> functionType)
	: name(name), Expression(functionType->returnType), functionType(functionType) {}

void Function::addParam(std::unique_ptr<Expression>&& param) {
	inputs.push_back(std::move(param));
}


std::tuple<std::string, std::shared_ptr<Memory>> Function::operator()(Scope& scope) {

	//add the name of the function, this is the push that occurs from call in assembly

	auto instructions = std::string();
	auto values = std::vector<std::shared_ptr<Memory>>();
	auto expression_instructions = std::vector<std::string>();
	int varadicParams = inputs.size() - functionType->parameters.size();

	for (auto& input : reverse(inputs)) {

		auto [instruction, value] = input->operator()(scope);

		//if varadic convert to default values
		if (functionType->isVaradic && varadicParams > 0) {


			/*if (value->dataType->id == (int)PrimitiveType::Bool
				|| value->dataType->id == (int)PrimitiveType::Char) {
				auto[convert_instructions, newValue] = value->
			}*/
			//convert float to double
			if (value->dataType->id == (int)PrimitiveType::Float) {
				auto [put_instruction, stack] = value->putInMemoryAddress(scope, value);
				instruction += put_instruction;
				instruction += "fld dword " + stack->toString() + "\n";
				instruction += "sub esp, byte 8\n";
				instruction += "fstp qword [esp]\n";

				value = std::make_shared<Stack>(scope.addUnnamed(8), std::make_shared<Type>(6, "double", 8));
			}
		}

		if (!value->isInMemoryAddress()) {
			auto [put_instructions, newValue] = value->putInMemoryAddress(scope, value);
			instruction += put_instructions;
			value = newValue;
		}

		values.push_back(value);

		instructions += instruction;
		varadicParams--;
	}

	int canSkip = 0;
	int totalSkipSize = 0;
	for (auto& value : values) {
		if (value->getType() == MemoryType::Stack) {
			canSkip++;
			totalSkipSize += value->dataType->size;
		}
		else {
			break;
		}
	}

	if (canSkip) {
		int offset = scope.getStackPtrCountBackWards(((Stack*)values[0].get())->offset);
		if (offset != canSkip) {
			//then we can't skip them
			canSkip = 0;
		}
	}



	for (size_t i = canSkip; i < values.size(); i++)
	{
		auto [push_instruction, new_val] = values[i]->putAtTopOfStack(scope, values[i]);
		instructions += push_instruction;
	}

	instructions += "call _" + name + '\n';
	instructions += "add esp, " + std::to_string(inputs.size() * 4) + '\n';

	//functions return value are in eax, which is the first 
	return std::make_tuple(instructions, std::make_unique<Regester>(0, dataType));
}