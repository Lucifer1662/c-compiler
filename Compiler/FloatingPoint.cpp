#include "FloatingPoint.h"
#include "NameAddress.h"
#include "Stack.h"
#include "FloatingStackManager.h"

FloatingPoint::FloatingPoint(std::shared_ptr<FloatingStackManager> stack, int offset)
	: stack(stack), offset(offset), Memory(std::make_shared<Type>(5, "double", 8)) {}

std::string FloatingPoint::toString() const {
	return "ST" + std::to_string(offset);
}

MemoryType FloatingPoint::getType() { return MemoryType::FloatingPointStack; }

std::tuple<std::string, std::shared_ptr<Memory>> FloatingPoint::putInRegester(Scope& scope, std::shared_ptr<Memory> memory) {
	auto [instructions, stack] = putInMemoryAddress(scope, memory);
	auto [reg_instructions, regester] = stack->putInRegester(scope, memory);

	return { instructions + reg_instructions, regester };
}

std::tuple<std::string, std::shared_ptr<Memory>> FloatingPoint::putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory) {
	auto [instructions, stack] = putInMemoryAddress(scope, memory);
	auto [reg_instructions, regester] = stack->putInRegester(scope, reg, memory);

	return { instructions + reg_instructions, regester };
}

std::tuple<std::string, std::shared_ptr<Memory>> FloatingPoint::putInMemoryAddress(Scope& scope, std::shared_ptr<Memory> memory) {
	auto stack = std::make_shared<Stack>(scope.addUnnamed(dataType->size), dataType);
	auto instructions = "sub    esp, byte " + std::to_string(stack->getSize()) + "\n";
	instructions += "fstp " + stack->getSizeName() + " " + stack->toString() + "\n";
	this->stack->swapToFirst(offset);
	this->stack->popOffStack();
	return { instructions, stack };
}

bool FloatingPoint::isInMemoryAddress() {
	return false;
}

std::tuple<std::string, std::shared_ptr<Memory>> FloatingPoint::putAtTopOfStack(Scope& scope, std::shared_ptr<Memory> memory)
{
	return putInMemoryAddress(scope, memory);
}

std::tuple<std::string, std::shared_ptr<Memory>> FloatingPoint::putAtTopOfStack(Scope& scope, std::string name, std::shared_ptr<Memory> memory)
{
	auto stack = std::make_shared<Stack>(scope.addName(name, dataType->size), dataType);
	auto instructions = "sub    esp, byte " + std::to_string(stack->getSize()) + "\n";

	
	if (dataType->id == (int)PrimitiveType::Float || dataType->id == (int)PrimitiveType::Double) {
		instructions += "fstp " + stack->getSizeName() + " " + stack->toString() + "\n";
	}
	else {
		instructions += "fistp " + stack->getSizeName() + " " + stack->toString() + "\n";
	}

	instructions += this->stack->swapToFirst(offset);
	this->stack->popOffStack();
	return { instructions, stack };
}
