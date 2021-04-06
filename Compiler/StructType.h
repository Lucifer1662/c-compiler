#pragma once
#include "Type.h"
#include <memory>
#include <vector>
#include <numeric>


template<typename BeginIt, typename EndIt>
int calcParamSize(const BeginIt& begin, const EndIt& end) {
	return std::accumulate(begin, end, 0, [](auto valueLeft, auto valueRight){
		return std::get<1>(valueRight)->size + valueLeft;
	});
}

struct StructType : public Type
{
	using Variables = std::vector<std::tuple<std::string, std::shared_ptr<Type>>>;
	Variables variables;

	StructType(int id, std::string name, Variables variables)
		: Type(id, name, calcParamSize(variables.begin(), variables.end())), variables(variables) {	}

	std::tuple<std::shared_ptr<Type>, int> getOffsetAndTypeForName(const std::string& name) {
		int offset = 0;
		for (auto [varName, varType] : variables) {
			if (varName == name) {
				return { varType, offset };
			}
			offset += varType->size;
		}
	}

};


