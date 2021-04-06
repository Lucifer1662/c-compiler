#include "Stack.h"
#include "RegesterManager.h"

std::string Stack::toString() const {
	if (offset > 0)
		return "[ebp + " + std::to_string(offset) + "]";
	else
		return "[ebp " + std::to_string(offset) + "]";
}

MemoryType Stack::getType() { return MemoryType::Stack; }

std::tuple<std::string, std::shared_ptr<Memory>> Stack::putInRegester(Scope& scope, std::shared_ptr<Memory> memory) {
	auto reg = std::make_shared<Regester>(dataType);
	auto instructions = "mov " + reg->toString() + ", " + toString() + '\n';

	return std::make_tuple(instructions, std::move(reg));
}

std::tuple<std::string, std::shared_ptr<Memory>> Stack::putInRegester(Scope& scope, int reg, std::shared_ptr<Memory> memory) {
	if (getFreeRegester(reg) == toString()) {
		return std::make_tuple("", memory);
	}

	auto instructions = std::string();
	instructions += "mov " + getFreeRegester(reg) + ", " + toString() + '\n';


	return std::make_tuple(instructions, std::make_shared<Regester>(reg, dataType));
}

//std::tuple<std::string, std::shared_ptr<Memory>> Stack::putAtTopOfStack(Scope& scope, std::shared_ptr<Memory> memory) {
//	if (offset != -scope.size) {
//		return  Memory::putAtTopOfStack(scope, memory);
//	}
//	return  std::make_tuple(std::string(), memory);
//}

std::tuple<std::string, std::shared_ptr<Memory>> Stack::putAtTopOfStack(Scope& scope, std::shared_ptr<Memory> memory)
{
	if (offset == -scope.size) {
		return { "", memory };
	}

	auto stack = std::make_shared<Stack>(scope.addUnnamed(), dataType);

	if (dataType->size == 4) {
		auto instructions = "push " + getSizeName() + " " + toString() + '\n';

		return std::make_tuple(instructions, stack);
	}
	else {
		auto instructions = std::string();
		auto regester = std::make_shared<Regester>(std::make_shared<Type>((int)PrimitiveType::Int, "int", 4));
		auto starOffset = offset + stack->getSize()-4;
		for (int i = 0; i < stack->getSize(); i += 4)
		{
			instructions += "push dword [ebp " + std::to_string(starOffset - i) + "] \n";
		}
		return std::make_tuple(instructions, stack);
	}
}


bool Stack::isInMemoryAddress() {
	return true;
}

