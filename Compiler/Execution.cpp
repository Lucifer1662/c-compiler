#include "Execution.h"

void Execution::addStatement(std::unique_ptr<Expression> expression) {
	statements.push_back(std::move(expression));
}

std::tuple<std::string, std::shared_ptr<Memory>> Execution::operator()(Scope& scope) {
	auto instructions = std::string();
	for (auto& exp : statements) {
		auto [instruction, _] = exp->operator()(scope);
		instructions += instruction;
	}

	return std::make_tuple(instructions, nullptr);
}

std::tuple<std::string, std::shared_ptr<Memory>> Execution::operator()(Scope& scope, std::shared_ptr<Memory>& reg) {
	auto instructions = std::string();
	for (auto& exp : statements) {
		auto [instruction, _] = exp->operator()(scope, reg);
		instructions += instruction;
	}

	return std::make_tuple(instructions, nullptr);
}