#pragma once
#include "GlobalVariable.h"
struct ConstantStringValue : public GlobalVariable {

	ConstantStringValue(std::string name, std::string value, std::shared_ptr<Type> dataType);
};

