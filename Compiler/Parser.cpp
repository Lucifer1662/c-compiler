#include "Parser.h"
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include "FunctionDeclaration.h"
#include "Function.h";
#include "VariableDeclaration.h"
#include "ConstantValue.h"
#include "Add.h"
#include "Assignment.h"
#include "ConstantStringValue.h"
#include "Return.h"
#include <algorithm>
#include "If.h"
#include "And.h"
#include "GreaterThan.h"
#include "Or.h"
#include "CmpMemory.h"
#include "BinaryComparison.h"
#include "While.h"
#include "ForLoop.h"
#include "IfElse.h"
#include "ConstantFloatValue.h"
#include "RegexHelpers.h"
#include "ArithmaticBinary.h"
#include "ArithmaticUnary.h"
#include "StructType.h"
#include "Dereference.h"















enum class ExpressionTypes {
	Global,
	FunctionDeclaration,
	VariableDeclarationAssignment,
	VariableDeclaration,
	FunctionCall,
	IntConstant,
	FloatConstant,
	StringLiteral,
	Paretheses,
	Plus,
	Multiplication,
	Division,
	Minus,
	Negate,
	And,
	Or,
	Assignment,
	Return,
	If,
	IfElse,
	Variable,
	Execution,
	ParethesesOpen,
	ParethesesClose,
	BraceOpen,
	BraceClose,
	Equal,
	NotEqual,
	WasZero,
	WasNotZero,
	GreaterThan,
	NotGreaterThan,
	GreaterThanOrEqual,
	NotGreaterThanOrEqual,
	LessThan,
	NotLessThan,
	LessThanOrEqual,
	NotLessThanOrEqual,
	IfFalse,
	IfTrue,
	While,
	ForLoop,
	PostIncrement,
	PreIncrement,
	PostDecrement,
	PreDecrement,
	StructDeclaration,
	Dot,
	Dereference,
	Comma
};



std::unique_ptr<Expression> Parser::parseScopeV2(const ScopeContent& scope) {


	switch ((ExpressionTypes)scope.type) {
	case ExpressionTypes::FunctionDeclaration: {
		auto name = std::string(functionName(scope.context));
		auto& params = scope.children[0]->children;
		auto paramsString = std::vector<std::string>();
		auto paramsTypes = std::vector<std::string_view>();
		paramsString.reserve(params.size());
		for (auto& param : params) {
			if (param->type != (int)ExpressionTypes::Comma) {
				paramsString.push_back(std::string(variableName(param->context)));
				paramsTypes.push_back(variableType(param->context));
			}
		}


		auto typeName = functionType(scope.context, paramsTypes);
		auto search = typeNameToTypeIndex.find(typeName);
		std::shared_ptr<FunctionType> type;
		if (search != typeNameToTypeIndex.end()) {
			type = std::static_pointer_cast<FunctionType>(search->second);
		}
		else {
			type = std::make_shared<FunctionType>(typeNameToTypeIndex.size(), name);
			typeNameToTypeIndex.emplace(typeName, type); 
			
		}
		variableNameToTypeIndex.emplace(name, type);


		auto funcDecl = std::make_unique<FunctionDeclaration>(name, paramsString, type);
		funcDecl->addStatement(parseScopeV2(*scope.children[1]));

		return funcDecl;
	}
	case ExpressionTypes::StructDeclaration: {
		auto name = std::string(functionName(scope.context));
		auto& variables = scope.children[0]->children;

		StructType::Variables variableTypes;
		for (auto& variable : variables) {
			auto varExpression = parseScopeV2(*variable);
			auto varName = std::string(variableName(variable->context));

			variableTypes.push_back(std::make_tuple(varName, varExpression->dataType) );
		}


		auto type = std::make_shared<StructType>(typeNameToTypeIndex.size(), name, variableTypes);

		typeNameToTypeIndex.emplace(name, type);
		variableNameToTypeIndex.emplace(name, type);

		return nullptr;
	}
	case ExpressionTypes::FunctionCall: {
		auto search = variableNameToTypeIndex.find(scope.context);
		std::shared_ptr<FunctionType> type;
		int size;
		if (search != variableNameToTypeIndex.end()) {
			type = std::static_pointer_cast<FunctionType>((search->second));
		}
		else {
			throw std::exception((std::string("Function '") + std::string(scope.context) + "' used before it being declared").c_str());
		}
		auto funcCall = std::make_unique<Function>(std::string(scope.context), type);

		auto& params = scope.children[0]->children;
		for (auto& child : params)
		{
			if(child->type != (int)ExpressionTypes::Comma)
				funcCall->addParam(parseScopeV2(*child));
		}
		return funcCall;
	}
	case ExpressionTypes::If: {
		return std::make_unique<If>(parseScopeV2(*scope.children[0]->children[0]), parseScopeV2(*scope.children[1]));
	}
	case ExpressionTypes::IfElse: {
		return std::make_unique<IfElse>(
			parseScopeV2(*scope.children[0]->children[0]->children[0]),
			parseScopeV2(*scope.children[0]->children[1]),
			parseScopeV2(*scope.children[1])
			);
	}
	case ExpressionTypes::While: {
		return std::make_unique<While>(parseScopeV2(*scope.children[0]->children[0]), parseScopeV2(*scope.children[1]));
	}
	case ExpressionTypes::ForLoop: {
		return std::make_unique<ForLoop>(
			parseScopeV2(*scope.children[0]->children[0]),
			parseScopeV2(*scope.children[0]->children[1]),
			parseScopeV2(*scope.children[0]->children[2]),
			parseScopeV2(*scope.children[1]));
	}
	case ExpressionTypes::Assignment: {
		return std::make_unique<Assignment>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]));
	}
	case ExpressionTypes::Plus: {
		return std::make_unique<ArithmaticBinary>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]), "add", "fadd");
	}
	case ExpressionTypes::Multiplication: {
		return std::make_unique<ArithmaticBinary>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]), "imul", "fmul");
	}
	case ExpressionTypes::Division: {
		return std::make_unique<ArithmaticBinary>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]), "idiv", "fdiv");
	}
	case ExpressionTypes::Minus: {
		if (scope.children.front()->type == (int)ExpressionTypes::Assignment) {
			scope.children.front()->parent = scope.parent;
		}
		else {
			return std::make_unique<ArithmaticBinary>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]), "sub", "fsub");
		}
	}
	case ExpressionTypes::Negate: {
		return std::make_unique<ArithmaticUnary>(parseScopeV2(*scope.children[0]), "neg", "fchs");
	}
	case ExpressionTypes::Dereference: {
		return std::make_unique<Dereference>(parseScopeV2(*scope.children[0]));
	}
	case ExpressionTypes::PreIncrement: {
		return std::make_unique<ArithmaticUnary>(parseScopeV2(*scope.children[0]), "inc", "");
	}
	case ExpressionTypes::PostIncrement: {
		return std::make_unique<ArithmaticUnary>(parseScopeV2(*scope.children[0]), "inc", "");
	}
	case ExpressionTypes::PreDecrement: {
		return std::make_unique<ArithmaticUnary>(parseScopeV2(*scope.children[0]), "dec", "");
	}
	case ExpressionTypes::PostDecrement: {
		return std::make_unique<ArithmaticUnary>(parseScopeV2(*scope.children[0]), "dec", "");
	}
	case ExpressionTypes::And: {
		return std::make_unique<And>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]));
	}
	case ExpressionTypes::Or: {
		return std::make_unique<Or>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]));
	}
	case ExpressionTypes::GreaterThan: {
		return std::make_unique<BinaryComparison>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]), CmpMemory::ComparisonEnum::GreaterThan);
	}
	case ExpressionTypes::GreaterThanOrEqual: {
		return std::make_unique<BinaryComparison>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]), CmpMemory::ComparisonEnum::GreaterThanOrEqual);
	}
	case ExpressionTypes::LessThan: {
		return std::make_unique<BinaryComparison>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]), CmpMemory::ComparisonEnum::LessThan);
	}
	case ExpressionTypes::LessThanOrEqual: {
		return std::make_unique<BinaryComparison>(parseScopeV2(*scope.children[0]), parseScopeV2(*scope.children[1]), CmpMemory::ComparisonEnum::LessThanOrEqual);
	}
	case ExpressionTypes::IntConstant: {
		auto search = typeNameToTypeIndex.find("int");
		std::shared_ptr<Type> type;
		if (search != typeNameToTypeIndex.end()) {
			type = search->second;
		}
		return std::make_unique<ConstantValue>(std::string(scope.context), type);
	}
	case ExpressionTypes::FloatConstant: {
		auto value = std::string(scope.context.substr(0, scope.context.size() - 1));
		auto h = std::hash<std::string>()(value);
		auto name = std::string("_FC") + std::to_string(h);
		auto search = typeNameToTypeIndex.find("float");
		std::shared_ptr<Type> type;
		if (search != typeNameToTypeIndex.end()) {
			type = search->second;
		}
		return std::make_unique<ConstantFloatValue>(name, std::string(value), type);
	}
	case ExpressionTypes::StringLiteral: {
		auto value = std::string(scope.context.substr(1, scope.context.size() - 2));
		auto h = std::hash<std::string>()(value);
		auto name = std::string("_SL") + std::to_string(h);
		return std::make_unique<ConstantStringValue>(name, value, typeNameToTypeIndex["char*"]);
	}
	case ExpressionTypes::Dot: {
		auto& left = scope.children[0];
		auto expr = parseScopeV2(*left);
		std::shared_ptr<StructType> type;
		if (expr) {
			type = std::static_pointer_cast<StructType>(expr->dataType);
		}
		else {
			auto search = variableNameToTypeIndex.find(left->context);
			if (search != variableNameToTypeIndex.end()) {
				type = std::static_pointer_cast<StructType>(search->second);
			}
			else {
				throw std::exception((std::string("Struct '") + std::string(scope.context) + "' used before it being declared").c_str());
			}
		}


		auto& right = scope.children[1];

		//now search in that type, for the right name
		auto [varType, offset] = type->getOffsetAndTypeForName(std::string(right->context));
		
		offset += ((Variable*)expr.get())->offset;
		auto name = std::string(((Variable*)expr.get())->name);
		name += std::string("_") + std::string(right->context);

		return std::make_unique<Variable>(name, varType, offset);
	}
	case ExpressionTypes::Variable: {
		auto search = variableNameToTypeIndex.find(scope.context);
		std::shared_ptr<Type> type;
		int size;
		if (search != variableNameToTypeIndex.end()) {
			type = search->second;
		}
		else {
			throw std::exception((std::string("Variable '") + std::string(scope.context) + "' used before it being declared").c_str());
		}
		return std::make_unique<Variable>(std::string(scope.context), type);
	}
	case ExpressionTypes::VariableDeclaration: {
		auto typeName = std::string(variableType(scope.context));
		//clean typeName of spaces
		std::remove_if(typeName.begin(), typeName.end(), isspace);

		auto name = variableName(scope.context);
		auto search = typeNameToTypeIndex.find(typeName);
		std::shared_ptr<Type> type;
		if (search != typeNameToTypeIndex.end()) {
			type = search->second;
		}
		variableNameToTypeIndex.emplace(name, type);
		return std::make_unique<VariableDeclaration>(std::string(name), type);
	}
	case ExpressionTypes::Return: {
		return std::make_unique<Return>(parseScopeV2(*scope.children[0]));
	}
	default: {
		if (scope.children.size() == 1) {
			return parseScopeV2(*scope.children[0]);
		}

		auto exec = std::make_unique<Execution>();
		for (auto& child : scope.children)
		{
			auto expression = parseScopeV2(*child);
			if(expression)
				exec->addStatement(std::move(expression));
		}
		return exec;
	}
	}


	return nullptr;
}



std::unique_ptr<Expression> Parser::parse()
{
	auto scope1 = ScopeContent();
	scope1.context = code;

	std::vector<std::tuple<int, std::regex>> flat2 = {
		{(int)ExpressionTypes::If, std::regex("if")},
		{(int)ExpressionTypes::IfElse, std::regex("else")},
		{(int)ExpressionTypes::While, std::regex("while")},
		{(int)ExpressionTypes::ForLoop, std::regex("for")},
		{(int)ExpressionTypes::Return, std::regex("return")},
		{(int)ExpressionTypes::StructDeclaration, std::regex("struct\\s+\\w+")},
		{(int)ExpressionTypes::FunctionDeclaration, std::regex("\\w+\\s+\\w+(?=\\s*\\()")},
		{(int)ExpressionTypes::FunctionCall, std::regex("\\w+(?=\\s*\\()")},
		{(int)ExpressionTypes::StringLiteral,  std::regex("\".*\"")},
		{(int)ExpressionTypes::FloatConstant, std::regex("\\d*(\\.\\d|\\d\\.)\\d*f(?!\\w)")},
		{(int)ExpressionTypes::IntConstant, std::regex("\\d+")},
		{(int)ExpressionTypes::Return, std::regex("return")},
		{(int)ExpressionTypes::VariableDeclaration, std::regex("\\w+(\\s+\\*\\s*|\\s*\\*\\s+|\\s+)\\w+")},
		{(int)ExpressionTypes::Variable, std::regex("\\w+")},
		{(int)ExpressionTypes::ParethesesOpen, std::regex("\\(")},
		{(int)ExpressionTypes::ParethesesClose, std::regex("\\)")},
		{(int)ExpressionTypes::BraceOpen, std::regex("\\{")},
		{(int)ExpressionTypes::BraceClose, std::regex("\\}")},
		{(int)ExpressionTypes::PreIncrement, std::regex("\\+\\+")},
		{(int)ExpressionTypes::PreDecrement, std::regex("--")},
		{(int)ExpressionTypes::And, std::regex("&&")},
		{(int)ExpressionTypes::Or, std::regex("\\|\\|")},
		{(int)ExpressionTypes::Plus, std::regex("\\+")},
		{(int)ExpressionTypes::Multiplication, std::regex("\\*")},
		{(int)ExpressionTypes::Division, std::regex("/")},
		{(int)ExpressionTypes::Minus, std::regex("-")},
		{(int)ExpressionTypes::GreaterThanOrEqual, std::regex(">\\=")},
		{(int)ExpressionTypes::GreaterThan, std::regex(">")},
		{(int)ExpressionTypes::LessThanOrEqual, std::regex("<\\=")},
		{(int)ExpressionTypes::LessThan, std::regex("<")},
		{(int)ExpressionTypes::Assignment, std::regex("\\=")},
		{(int)ExpressionTypes::Dot, std::regex("\\.")},
		{(int)ExpressionTypes::Comma, std::regex(",")},

	};

	scope1.parseTextFlat(flat2);
	scope1.removeUnclassifiedLeafs();
	for (auto& child : scope1.children) {
		auto s = std::string(child->context);
		auto t = child->type;
		std::cout << t << ", " << s << std::endl;
		int j = 0;
	}

	std::vector<std::tuple<int, int>> flat3 = {
		{(int)ExpressionTypes::ParethesesOpen, (int)ExpressionTypes::ParethesesClose},
		{(int)ExpressionTypes::BraceOpen,(int)ExpressionTypes::BraceClose}
	};

	scope1.parseTextV2(flat3);

	scope1.resolveAmbiguousSymbols((int)ExpressionTypes::Minus, (int)ExpressionTypes::Negate,
		[](auto begin, auto end) { 
		auto type = (*begin)->type;
		return type != (int)ExpressionTypes::BraceClose
			&& type != (int)ExpressionTypes::FloatConstant
			&& type != (int)ExpressionTypes::FunctionCall
			&& type != (int)ExpressionTypes::IntConstant
			&& type != (int)ExpressionTypes::ParethesesClose
			&& type != (int)ExpressionTypes::StringLiteral
			&& type != (int)ExpressionTypes::Variable;
		},
		[](auto begin, auto end) { return false; }
	);

	scope1.resolveAmbiguousSymbols((int)ExpressionTypes::PreIncrement, (int)ExpressionTypes::PostIncrement,
		[](auto begin, auto end) {
		auto type = (*begin)->type;
		return type == (int)ExpressionTypes::Variable;
	},
		[](auto begin, auto end) { return false; }
	);

	scope1.resolveAmbiguousSymbols((int)ExpressionTypes::PreDecrement, (int)ExpressionTypes::PostDecrement,
		[](auto begin, auto end) {
		auto type = (*begin)->type;
		return type == (int)ExpressionTypes::Variable;
	},
		[](auto begin, auto end) { return false; }
	);

	scope1.resolveAmbiguousSymbols((int)ExpressionTypes::Multiplication, (int)ExpressionTypes::Dereference,
		[](auto begin, auto end) {
		auto type = (*begin)->type;
		return type != (int)ExpressionTypes::BraceClose
			&& type != (int)ExpressionTypes::FloatConstant
			&& type != (int)ExpressionTypes::FunctionCall
			&& type != (int)ExpressionTypes::IntConstant
			&& type != (int)ExpressionTypes::ParethesesClose
			&& type != (int)ExpressionTypes::StringLiteral
			&& type != (int)ExpressionTypes::Variable;
	},
		[](auto begin, auto end) { return false; }
	);
	

	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::Dot, 1, 1 });
	//scope1.makeAdjacenetChildren({ (int)ExpressionTypes::Dereference, 0, 1 });
	scope1.makeAdjacenetChildrenFunc(
		(int)ExpressionTypes::Dereference,
		[](auto begin, auto end) { return 0; } ,
		[](auto begin, auto end) { 
		int i = 0;
		while (begin != end && (*begin)->type == (int)ExpressionTypes::Dereference ) {
			i++;
			begin++;
		}

		return i+1;
	} );
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::PreIncrement, 0, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::PostIncrement, 1, 0 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::PreDecrement, 0, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::PostDecrement, 1, 0 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::Negate, 0, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::Plus, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::Multiplication, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::Division, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::Minus, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::FunctionCall, 0, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::If, 0, 2 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::IfElse, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::While, 0, 2 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::ForLoop, 0, 2 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::GreaterThan, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::GreaterThanOrEqual, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::LessThan, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::LessThanOrEqual, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::And, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::Or, 1, 1 });

	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::FunctionDeclaration, 0, 2 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::StructDeclaration, 0, 1 });

	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::Assignment, 1, 1 });
	scope1.makeAdjacenetChildren({ (int)ExpressionTypes::Return, 0, 1 });

	auto result = parseScopeV2(scope1);
	return result;
}
