#pragma once
#include <string>
#include <memory>
#include "Expression.h"
#include "ScopeContent.h";
#include <unordered_map>
#include "Type.h"
#include "FunctionType.h"

struct Parser
{
	Parser(std::string code) : code(code) {

		{
			auto typeName = std::string("printf_charptr_@Vargs_");
			auto printfType = std::make_shared<FunctionType>((int)typeNameToTypeIndex.size(), typeName, true);
			printfType->parameters.push_back(typeNameToTypeIndex["char*"]);
			printfType->returnType = typeNameToTypeIndex["int"];
			typeNameToTypeIndex.emplace(typeName, printfType);
			variableNameToTypeIndex.emplace("printf", printfType);
		}
		

		{
			auto typeName = std::string("malloc_int_");
			auto mallocType = std::make_shared<FunctionType>((int)typeNameToTypeIndex.size(), typeName);
			mallocType->parameters.push_back(typeNameToTypeIndex["int"]);
			mallocType->returnType = typeNameToTypeIndex["void*"];
			typeNameToTypeIndex.emplace(typeName, mallocType);
			variableNameToTypeIndex.emplace("malloc", mallocType);
		}

		{
			auto typeName = std::string("free_int_");
			auto mallocType = std::make_shared<FunctionType>((int)typeNameToTypeIndex.size(), typeName);
			mallocType->parameters.push_back(typeNameToTypeIndex["int"]);
			mallocType->returnType = typeNameToTypeIndex["void"];
			typeNameToTypeIndex.emplace(typeName, mallocType);
			variableNameToTypeIndex.emplace("free", mallocType);
		}
	
	}

	std::string code;
	std::unordered_map<std::string, std::shared_ptr<Type>> typeNameToTypeIndex = { {
		{"void", std::make_shared<Type>(0,"void",0)},
		{"bool",std::make_shared<Type>(1,"bool",1)},
		{"char",std::make_shared<Type>(2,"char",1)},
		{"int",std::make_shared<Type>(3,"int",4)},
		{"float",std::make_shared<Type>(4,"float",4)},
		{"double",std::make_shared<Type>(5,"double",8)},
		{"void*",std::make_shared<Type>(6,"void*",4,1)},
		{"bool*",std::make_shared<Type>(7,"bool*",4,1)},
		{"char*",std::make_shared<Type>(8,"char*",4,1)},
		{"int*",std::make_shared<Type>(9,"int*",4,1)},
		{"float*",std::make_shared<Type>(10,"float*",4,1)},
		{"double*",std::make_shared<Type>(11,"double*",4,1)},
		
		} };

	std::unordered_map<std::string_view, std::shared_ptr<Type>> variableNameToTypeIndex;
	std::unique_ptr<Expression> parse();
	std::unique_ptr<Expression> parseScopeV2(const ScopeContent& scope);
};

