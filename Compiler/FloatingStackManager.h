#pragma once
#include <vector>
#include <memory>
#include "Memory.h"
struct FloatingPoint;

struct FloatingStackManager
{
	std::vector<std::weak_ptr<FloatingPoint>> stack = 
		std::vector<std::weak_ptr<FloatingPoint>>(8);
	
public:
	std::weak_ptr<FloatingStackManager> self;
	FloatingStackManager() {}

	static std::shared_ptr<FloatingStackManager> make_FloatStackManager();

	int getFreeFloat();

	void shiftStackUp();

	void shiftStackDown();


	std::shared_ptr<FloatingPoint> putOnStack();

	std::shared_ptr<FloatingPoint> popOffStack();
	std::shared_ptr<FloatingPoint> overwriteAt(int index);
	std::string swapToFirst(int index);

	std::tuple<std::string, std::shared_ptr<FloatingPoint>> add(Scope& scope, std::shared_ptr<Memory> left, std::shared_ptr<Memory> right);
	std::tuple<std::string, std::shared_ptr<FloatingPoint>> operation(Scope& scope, std::shared_ptr<Memory> left, std::shared_ptr<Memory> right, std::string operation);
	std::tuple<std::string, std::shared_ptr<FloatingPoint>> operation(Scope& scope, std::shared_ptr<Memory> left, std::string operation);
};

