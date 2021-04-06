#pragma once
#include "GlobalVariable.h"

struct ConstantFloatValue : public GlobalVariable 
{
	ConstantFloatValue(std::string name, std::string value, std::shared_ptr<Type> dataType);

	std::tuple<std::string, std::shared_ptr<Memory>> operator()(Scope& scope);
};

