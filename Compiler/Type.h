#pragma once
#include <string_view>
struct Type
{
	int id;
	std::string name;
	int size;
	int ptrCount;

	Type(int id, std::string_view name, int size, int ptrCount = 0)
		:id(id), name(name), size(size), ptrCount(ptrCount){}
};


enum class PrimitiveType {
	Void,
	Bool,
	Char,
	Int,
	Float,
	Double

};
