#pragma once
#include "BinaryOperator.h"
#include <memory>
#include "CmpMemory.h"

struct BinaryComparison : public BinaryOperator
{
	CmpMemory::ComparisonEnum operation;

	BinaryComparison(std::unique_ptr<Expression>&& left,
		std::unique_ptr<Expression>&& right,
		CmpMemory::ComparisonEnum operation);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);

};

