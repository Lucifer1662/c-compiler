#pragma once
#include <string>
#include "MemoryType.h"
#include <tuple>
#include <memory>
#include "Type.h"

struct Scope;

struct Memory {
	bool destroyed = false;
	bool dereference = false;
	void Destroy();

	bool isDestroyed();

	int getSize();

	std::string getSizeName();
	std::shared_ptr<Type> dataType;

	Memory(std::shared_ptr<Type> dataType, bool dereference = false) 
		: dataType(dataType), dereference(dereference)
	{};

	std::string addDereference(std::string name);

	void Dereference() { dereference = true; }
	void Makereference() { dereference = false;  }
	bool isDereference() { return dereference; }


	virtual std::string toString() const;

	virtual void save();

	virtual std::string load(std::string instructions);

	virtual MemoryType getType();

	virtual std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, std::shared_ptr<Memory> memory) = 0;

	virtual std::tuple<std::string, std::shared_ptr<Memory>> putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory) = 0;

	virtual std::tuple<std::string, std::shared_ptr<Memory>> putInMemoryAddress(Scope& scope, std::shared_ptr<Memory> memory);

	virtual std::tuple<std::string, std::shared_ptr<Memory>> putAtTopOfStack(Scope& scope, std::shared_ptr<Memory> memory);

	virtual std::tuple<std::string, std::shared_ptr<Memory>> putAtTopOfStack(Scope& scope, std::string name, std::shared_ptr<Memory> memory);

	virtual std::tuple<std::string, std::shared_ptr<Memory>> loadToFPU(Scope& scope, std::shared_ptr<Memory> memory);

	virtual bool isInMemoryAddress();
};





