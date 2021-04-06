#pragma once
#include "Memory.h"
#include <tuple>

struct FloatingStackManager;

struct FloatingPoint : public Memory
{
	int offset;
	std::shared_ptr<FloatingStackManager> stack;

	FloatingPoint(std::shared_ptr<FloatingStackManager> stack, int offset);


	std::string toString() const;

	MemoryType getType();

	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, std::shared_ptr<Memory> memory);
	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory);

	std::tuple<std::string, std::shared_ptr<Memory>> putInMemoryAddress(Scope& scope, std::shared_ptr<Memory> memory);

	bool isInMemoryAddress();

	std::tuple<std::string, std::shared_ptr<Memory>> putAtTopOfStack(Scope& scope, std::shared_ptr<Memory> memory);

	std::tuple<std::string, std::shared_ptr<Memory>> putAtTopOfStack(Scope& scope, std::string name, std::shared_ptr<Memory> memory);


};

