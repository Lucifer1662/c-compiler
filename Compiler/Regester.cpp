#include "Regester.h"
#include "ReservedReg.h"
#include "RegesterManager.h"

 Regester::Regester(int reg, std::shared_ptr<Type> dataType) :reg(reg), Memory(dataType){
	makeRegesterFree(reg);
}

Regester::Regester(std::shared_ptr<Type> dataType) : Memory(dataType) {
	reg = getFreeRegesterIndex();
}

std::string Regester::toString() const {
	if(dereference)
		return "[" + getFreeRegester(reg) + "]";
	else
		return getFreeRegester(reg);
}

 void Regester::save() {
	reserver = reserveReg(reg);
}

 std::string Regester::load(std::string instructions) {
	auto [new_instructions, new_reg] = reserver->wasRegReserved(instructions);
	reg = new_reg;
	return new_instructions;
}

 std::tuple<std::string, std::shared_ptr<Memory>> Regester::putInRegester(Scope& scope, std::shared_ptr<Memory> memory) {
	return std::make_tuple(std::string(), memory);
}

 std::tuple<std::string, std::shared_ptr<Memory>> Regester::putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory) {
	if (getFreeRegester(reg) == toString()) {
		return std::make_tuple("", memory);
	}

	auto instructions = std::string();
	instructions += "mov " + getFreeRegester(reg) + ", " + toString() + '\n';


	return std::make_tuple(instructions, std::make_unique<Regester>(reg, dataType));


}


 std::tuple<std::string, std::shared_ptr<Memory>> Regester::loadToFPU(Scope& scope, std::shared_ptr<Memory> memory)
 {
	std::string instructions;
	auto [putInstructions, stack] = putAtTopOfStack(scope, memory);
	instructions += putInstructions;

	auto [loadInstructions, floating] = stack->loadToFPU(scope, stack);
	instructions += loadInstructions;

	return { instructions, floating };
 }