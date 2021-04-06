#pragma once
#include "Type.h"
#include <memory>
#include <vector>


struct FunctionType : public Type
{
	//size is my paramters
	std::vector<std::shared_ptr<Type>> parameters;
	std::shared_ptr<Type> returnType;
	bool isVaradic;

	FunctionType(int id, std::string name, bool isVaradic=false) : Type(id, name, 4), isVaradic(isVaradic) {}

};

