#pragma once
#include "ExpressionEnum.h"
#include <tuple>
#include <string>
#include <memory>
#include "Memory.h"
#include "Scope.h"

struct Expression {
	ExpressionEnum type;
	enum DataType {
		Void,
		Bool,
		Char,
		Int,
		Float,
		Double,
	};

	std::shared_ptr<Type> dataType;

	Expression(std::shared_ptr<Type> dataType) :dataType(dataType) {}
	Expression(): dataType(nullptr) {}

	virtual std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope) {
		return std::make_tuple(std::string(), nullptr);
	}

	virtual std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope, std::shared_ptr<Memory>& reg) {
		return std::make_tuple(std::string(), nullptr);
	}
};
