#pragma once
#include "MemoryType.h"
#include <string>
#include "Memory.h"
#include <memory>
#include "Scope.h"
struct ReserveReg;

struct Regester : public Memory {
	int reg;

	Regester(int reg, std::shared_ptr<Type> dataType);

	Regester(std::shared_ptr<Type> dataType);

	std::shared_ptr<ReserveReg> reserver;
	std::string toString() const override;

	void save();

	std::string load(std::string instructions);

	MemoryType getType() { return MemoryType::Regester; }

	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, std::shared_ptr<Memory> memory);

	std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory);
	std::tuple<std::string, std::shared_ptr<Memory>> loadToFPU(Scope& scope, std::shared_ptr<Memory> memory);
};


