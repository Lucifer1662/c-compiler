#pragma once
#include <string>
#include "Expression.h"
#include "NameAddress.h"

const std::vector<std::string> sizeNames = {
	"db", "dw", "dd"
};

struct GlobalVariable : public Expression {
	std::string name;
	std::string value;

	enum class Size {
		Byte,
		Byte2,
		Byte4
	};

	Size size;



	GlobalVariable(std::string name, std::string value, Size size, std::shared_ptr<Type> dataType) : name(name), value(value), size(size), Expression(dataType) {}

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope) {

		auto reg = std::make_unique<NameAddress>(name, dataType);

		auto instructions = name;
		instructions += " " + sizeNames[(int)size] + " " + value + "\n";

		scope.addToGlobal(name, instructions);

		return std::make_tuple("", std::move(reg));
	}
};
