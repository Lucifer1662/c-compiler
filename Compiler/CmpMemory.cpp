#include "CmpMemory.h"
#include "Scope.h"
#include "Regester.h"
#include "Stack.h"


const char* CmpMemory::ComparisonNames[] = {
"je",
"jne",
"jz",
"jns",
"jg",
"jng",
"jge",
"jnge",
"jl",
"jnl",
"jle",
"jnle",
"je",
"jne",
};



CmpMemory::CmpMemory(ComparisonEnum comparison, std::shared_ptr<Type> dataType) :
	comparison(comparison), Memory(dataType)
{}

std::string CmpMemory::getComparisonString()
{
	return ComparisonNames[(int)comparison];
}

std::string CmpMemory::getNotComparisonString()
{
	if((int)comparison%2 == 0)
		return ComparisonNames[(int)comparison+1];
	else
		return ComparisonNames[(int)comparison - 1];
}

std::string CmpMemory::toString() const
{
	return std::string();
}

MemoryType CmpMemory::getType()
{
	return MemoryType::CmpMemory;
}

std::tuple<std::string, std::shared_ptr<Memory>> CmpMemory::putInRegester(Scope& scope, std::shared_ptr<Memory> memory)
{
	auto endLabel = scope.getUniqueLabel();
	auto reg = std::make_shared<Regester>(dataType);

	auto instructions = std::string();
	instructions += "mov " + reg->toString() + ", 1\n";
	instructions +=  getComparisonString() + " " + endLabel.use() + " \n";
	instructions += "mov " + reg->toString() + ", 0\n";
	instructions += endLabel.declareLabel();


	return std::make_tuple(instructions, std::move(reg));
}

std::tuple<std::string, std::shared_ptr<Memory>> CmpMemory::putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory)
{
	auto endLabel = scope.getUniqueLabel();
	auto regester = std::make_shared<Regester>(reg, dataType);

	auto instructions = std::string();
	instructions += "je " + endLabel.use() + " \n";
	instructions += "mov " + regester->toString() + " 1\n";
	instructions += endLabel.declareLabel();
	instructions += "mov " + regester->toString() + " 0\n";


	return std::make_tuple(instructions, std::move(regester));
}


std::tuple<std::string, std::shared_ptr<Memory>> CmpMemory::putAtTopOfStack(Scope& scope, std::shared_ptr<Memory> memory)
{
	std::string instructions;

	auto [put_instructions, regester] = putInRegester(scope, memory);
	instructions += put_instructions;

	auto [topStack_instructions, stack] = regester->putAtTopOfStack(scope, memory);
	instructions += topStack_instructions;

	return std::make_tuple(instructions, stack);
}

std::tuple<std::string, std::shared_ptr<Memory>> CmpMemory::putAtTopOfStack(Scope& scope, std::string name, std::shared_ptr<Memory> memory)
{
	std::string instructions;

	auto [put_instructions, regester] = putInRegester(scope, memory);
	instructions += put_instructions;

	auto [topStack_instructions, stack] = regester->putAtTopOfStack(scope, name, memory);
	instructions += topStack_instructions;

	return std::make_tuple(instructions, stack);
}