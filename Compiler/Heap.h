#pragma once
#include "Memory.h"
#include <memory>
#include "Regester.h"

struct Heap : public Memory {

	MemoryType getType();

	std::string toString() const;
	
	Heap(std::shared_ptr<Type> dataType):Memory(dataType){}

	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, std::shared_ptr<Memory> memory);

	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory);

	std::tuple<std::string, std::shared_ptr<Memory>> putInMemoryAddress(Scope& scope, std::shared_ptr<Memory> memory) {
		return  std::make_tuple("", memory);
	}

	bool isInMemoryAddress() {
		return true;
	}
};

