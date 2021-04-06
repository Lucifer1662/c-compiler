#pragma once
#include "Memory.h"

struct CmpMemory : public Memory
{
	enum class ComparisonEnum {
		Equal,
		NotEqual,
		WasZero,
		WasNotZero,
		GreaterThan,
		NotGreaterThan,
		GreaterThanOrEqual,
		NotGreaterThanOrEqual,
		LessThan,
		NotLessThan,
		LessThanOrEqual,
		NotLessThanOrEqual,
		IfFalse,
		IfTrue
	};

	static const char* ComparisonNames[];

	ComparisonEnum comparison;

	CmpMemory(ComparisonEnum comparison, std::shared_ptr<Type> dataType);

	std::string getComparisonString();
	std::string getNotComparisonString();

	std::string toString() const override;

	MemoryType getType();

	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, std::shared_ptr<Memory> memory);


	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory);

	std::tuple<std::string, std::shared_ptr<Memory>> putAtTopOfStack(Scope& scope, std::shared_ptr<Memory> memory);

	std::tuple<std::string, std::shared_ptr<Memory>> putAtTopOfStack(Scope& scope, std::string name, std::shared_ptr<Memory> memory);
};

