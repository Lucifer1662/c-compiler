#pragma once
#include "Memory.h"
#include <string>
#include <memory>
#include "Regester.h"

struct Stack : public Memory {
	int offset;

	Stack(int offset, std::shared_ptr<Type> dataType) :offset(offset), Memory(dataType, true) {	}

	std::string toString() const override;

	MemoryType getType();

	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, std::shared_ptr<Memory> memory);


	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory);

	std::tuple<std::string, std::shared_ptr<Memory>> putInMemoryAddress(Scope& scope, std::shared_ptr<Memory> memory) {
		return  std::make_tuple(std::string(), std::make_shared<Stack>(offset, dataType));
	}

	std::tuple<std::string, std::shared_ptr<Memory>> putAtTopOfStack(Scope& scope, std::shared_ptr<Memory> memory);

	bool isInMemoryAddress();
};


