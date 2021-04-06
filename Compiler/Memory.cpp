#include "Memory.h"
#include "Memory.h"
#include "Stack.h"
#include "FloatingPoint.h"

void Memory::Destroy() {
	destroyed = true;
}

bool Memory::isDestroyed() {
	return destroyed;
}

int Memory::getSize() {
	if (dataType)
		return dataType->size;
}

std::string Memory::getSizeName() {
	if (dataType)
		switch (dataType->size) {
		case 1: return "byte";
		case 2: return "word";
		case 4: return "dword";
		case 8: return "qword";
		}
	return "dword";
}

std::string Memory::addDereference(std::string name)
{
	auto start = std::string();
	start.reserve(dataType->ptrCount);
	for (size_t i = 0; i < dataType->ptrCount; i++)
	{
		start.append("[");
	}

	auto end = std::string();
	end.reserve(dataType->ptrCount);
	for (size_t i = 0; i < dataType->ptrCount; i++)
	{
		end.append("]");
	}

	return start + name + end;
	
}

std::string Memory::toString() const {
	return "";
}

void Memory::save() {

}

std::string Memory::load(std::string instructions) {
	return instructions;
}

MemoryType Memory::getType() {
	return MemoryType::NotInMemory;
}

std::tuple<std::string, std::shared_ptr<Memory>> Memory::putInMemoryAddress(Scope& scope, std::shared_ptr<Memory> memory)
{	
	return putAtTopOfStack(scope, memory);
	/*auto reg = std::make_shared<Stack>(scope.addUnnamed(dataType->size), dataType);
	auto instructions = "mov " + reg->toString() + ", " + toString() + '\n';
	return std::make_tuple(instructions, reg);*/
}

std::tuple<std::string, std::shared_ptr<Memory>> Memory::putAtTopOfStack(Scope& scope, std::shared_ptr<Memory> memory)
{
	auto stack = std::make_shared<Stack>(scope.addUnnamed(dataType->size), dataType);

	auto instructions = "push " + getSizeName() + " " + toString() + '\n';

	return std::make_tuple(instructions, stack);
}

std::tuple<std::string, std::shared_ptr<Memory>> Memory::putAtTopOfStack(Scope& scope, std::string name, std::shared_ptr<Memory> memory)
{
	auto stack = std::make_shared<Stack>(scope.addName(name, dataType->size), dataType);

	auto instructions = "push " + getSizeName() + " " + toString() + '\n';

	return std::make_tuple(instructions, stack);
}

std::tuple<std::string, std::shared_ptr<Memory>> Memory::loadToFPU(Scope& scope, std::shared_ptr<Memory> memory)
{
	auto floating = scope.floatingStack->putOnStack();
	auto instruction = std::string();
	if (dataType->id == (int)PrimitiveType::Float || dataType->id == (int)PrimitiveType::Double) {
		instruction = std::string("fld ") + getSizeName() + " " + toString() + '\n';
	}
	else {
		instruction = std::string("fild ") + getSizeName() + " " + toString() + '\n';
	}
	return { instruction, floating };
}

bool Memory::isInMemoryAddress()
{
	return false;
}

