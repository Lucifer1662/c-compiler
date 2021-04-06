#pragma once
#include "Memory.h"
#include <memory>
#include "Regester.h"

struct NameAddress : public Memory {

	std::string name;

	NameAddress(std::string name, std::shared_ptr<Type> dataType);


	std::string toString() const;

	MemoryType getType();

	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, std::shared_ptr<Memory> memory);
	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory);

	std::tuple<std::string, std::shared_ptr<Memory>> putInMemoryAddress(Scope& scope, std::shared_ptr<Memory> memory) {
		return  std::make_tuple(std::string(), memory);
	}

	bool isInMemoryAddress() {
		return true;
	}
};

