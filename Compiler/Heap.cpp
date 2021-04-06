#include "Heap.h"

MemoryType Heap::getType() { return MemoryType::Heap; }

std::tuple<std::string, std::shared_ptr<Memory>> Heap::putInRegester(Scope& scope, std::shared_ptr<Memory> memory) {
	auto reg = std::make_unique<Regester>(dataType);
	auto instructions = "mov " + reg->toString() + ", " + toString() + '\n';

	return std::make_tuple(instructions, std::move(reg));
}

std::tuple<std::string, std::shared_ptr<Memory>> Heap::putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory)
{
	return putInRegester(scope, memory);
}


std::string Heap::toString() const {
	return "[Some Constant]";
}