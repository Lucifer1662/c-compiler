#include "FloatingStackManager.h"
#include "Scope.h"
#include "FloatingPoint.h"

std::shared_ptr<FloatingStackManager> FloatingStackManager::make_FloatStackManager() {
	auto stackManager = std::make_shared<FloatingStackManager>();
	stackManager->self = stackManager;
	return stackManager;
}

int FloatingStackManager::getFreeFloat() {
	size_t i;
	for (i = 0; i < stack.size(); i++)
	{
		if (stack[i].expired()) {
			return i;
		}
	}
	//else swap
	i = rand() % stack.size();
	stack[i].lock()->Destroy();
	return i;
}

void FloatingStackManager::shiftStackUp() {
	if (!stack.back().expired())
		stack.back().lock()->Destroy();

	for (size_t i = 0; i < stack.size(); i++)
	{
		if(!stack[i].expired())
			stack[i].lock()->offset++;
	}

	for (size_t i = stack.size() - 1; i >= 1; i--) {
		stack[i] = stack[i - 1];
	}
}

void FloatingStackManager::shiftStackDown() {
	auto start = stack.begin()->lock();

	for (size_t i = 0; i < stack.size(); i++)
	{
		if (!stack[i].expired())
			stack[i].lock()->offset--;
	}

	for (size_t i = 0; i < stack.size() - 1; i++) {
		stack[i] = stack[i + 1];
	}

	stack.back() = start;
}

std::shared_ptr<FloatingPoint> FloatingStackManager::putOnStack() {
	shiftStackUp();
	auto floating = std::make_shared<FloatingPoint>(self.lock(), 0);
	stack[0] = floating;
	return floating;
}

std::shared_ptr<FloatingPoint> FloatingStackManager::popOffStack() {
	shiftStackDown();
	return stack.back().lock();
}

std::shared_ptr<FloatingPoint> FloatingStackManager::overwriteAt(int index)
{
	if (!stack[index].expired())
		stack[index].lock()->Destroy();
	auto newValue = std::make_shared<FloatingPoint>(self.lock(), index);
	stack[index] = newValue;
	return newValue;
}

std::string FloatingStackManager::swapToFirst(int index)
{
	if (index != 0) {
		auto oldFirst = stack.front().lock();
		stack.front() = stack[index];
		stack.front().lock()->offset = 0;
		stack[index] = oldFirst;

		oldFirst->offset = index;

		return std::string("fxch ") + stack[index].lock()->toString() + "\n";
	}
	else {
		return "";
	}
}

std::tuple<std::string, std::shared_ptr<FloatingPoint>> FloatingStackManager::add(Scope& scope, std::shared_ptr<Memory> left, std::shared_ptr<Memory> right) {
	auto instruction = std::string();

	auto loadToFPU = [&](std::shared_ptr<Memory>& value) {
		auto [load_left_instructions, leftFloat] = value->loadToFPU(scope, value);
		instruction += load_left_instructions;
		return leftFloat;
	};

	auto addToFPU = [&](std::shared_ptr<Memory>& value) {
		auto [putInstructions, newValue] = value->putInMemoryAddress(scope, value);

		instruction += "fadd " + newValue->getSizeName() + " " + newValue->toString() + '\n';
		return overwriteAt(0);
	};

	std::shared_ptr<Memory> loaded;
	std::shared_ptr<FloatingPoint> result;
	if (left->getType() != MemoryType::FloatingPointStack && right->getType() == MemoryType::FloatingPointStack) {
		loaded = loadToFPU(right);
		result = addToFPU(left);
	}
	else {
		loaded = loadToFPU(left);
		result = addToFPU(right);
	}

	return { instruction, result };
}

std::tuple<std::string, std::shared_ptr<FloatingPoint>> FloatingStackManager::operation(Scope& scope, std::shared_ptr<Memory> left, std::shared_ptr<Memory> right, std::string operation)
{
	auto instruction = std::string();

	auto loadToFPU = [&](std::shared_ptr<Memory>& value) {
		auto [load_left_instructions, leftFloat] = value->loadToFPU(scope, value);
		instruction += load_left_instructions;
		return leftFloat;
	};

	auto opOnFPU = [&](std::shared_ptr<Memory>& value) {
		auto [putInstructions, newValue] = value->putInMemoryAddress(scope, value);

		instruction += operation + " " + newValue->getSizeName() + " " + newValue->toString() + '\n';
		return overwriteAt(0);
	};

	std::shared_ptr<Memory> loaded;
	std::shared_ptr<FloatingPoint> result;
	if (left->getType() != MemoryType::FloatingPointStack && right->getType() == MemoryType::FloatingPointStack) {
		loaded = loadToFPU(right);
		result = opOnFPU(left);
	}
	else {
		loaded = loadToFPU(left);
		result = opOnFPU(right);
	}

	return { instruction, result };
}

std::tuple<std::string, std::shared_ptr<FloatingPoint>> FloatingStackManager::operation(Scope& scope, std::shared_ptr<Memory> left, std::string operation)
{
	auto instruction = std::string();

	auto loadToFPU = [&](std::shared_ptr<Memory>& value) {
		auto [load_left_instructions, leftFloat] = value->loadToFPU(scope, value);
		instruction += load_left_instructions;
		return leftFloat;
	};

	auto opOnFPU = [&](std::shared_ptr<Memory>& value) {
		instruction += operation + '\n';
		return overwriteAt(0);
	};

	std::shared_ptr<Memory> loaded;
	std::shared_ptr<FloatingPoint> result;

	loaded = loadToFPU(left);
	result = opOnFPU(left);
	


	return { instruction, result };
}
