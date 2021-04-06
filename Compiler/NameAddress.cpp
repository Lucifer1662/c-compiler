#include "NameAddress.h"
#include "RegesterManager.h"
NameAddress::NameAddress(std::string name, std::shared_ptr<Type> dataType) : Memory(dataType), name(name) {}

std::string NameAddress::toString() const {
	return name;
}

MemoryType NameAddress::getType() { return MemoryType::NameAddress; }

std::tuple<std::string, std::shared_ptr<Memory>> NameAddress::putInRegester(Scope& scope, std::shared_ptr<Memory> memory) {
	auto reg = std::make_shared<Regester>(dataType);
	auto instructions = "mov " + reg->toString() + ", " + toString() + '\n';

	return std::make_tuple(instructions, std::move(reg));
}

std::tuple<std::string, std::shared_ptr<Memory>> NameAddress::putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory) {
	if (getFreeRegester(reg) == toString()) {
		return std::make_tuple("", memory);
	}

	auto instructions = std::string();
	instructions += "mov " + getFreeRegester(reg) + ", " + toString() + '\n';

	return std::make_tuple(instructions, std::make_shared<Regester>(reg, dataType));
}

