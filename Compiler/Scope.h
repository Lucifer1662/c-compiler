#pragma once
#include <vector>
#include <string>
#include <memory>
#include <optional>
#include "ScopeLabels.h"
#include <map>
#include "FloatingStackManager.h"

struct StackVar {
	std::string name;
	int size;
	StackVar(std::string name, size_t size): name(name), size(size){}
	StackVar(size_t size) : name(""), size(size) {}
};

struct Scope {
	std::vector<StackVar> names = std::vector<StackVar>();
	int size = 0;
	Scope* parent;
	std::shared_ptr<int> globalCompileNameCount;
	std::shared_ptr<std::map<std::string, std::string>> globalNames;
	std::shared_ptr<FloatingStackManager> floatingStack;

	enum class ScopeEnum {
		Global,
		FunctionCall,
		Label,
	};

	ScopeEnum type;

	/*std::optional<std::tuple<int, int>> labelScope;
	bool usedFalseLabel = false;
	bool usedTrueLabel = false;
	Scope* trueLabelOwner = nullptr;
	Scope* falseLabelOwner = nullptr;*/

private:
	std::optional<ScopeLabels> labels;
public:

	Scope();
	Scope(Scope* scope);

	Scope childScope();
	Scope createFunctionScope();

	int addName(std::string name, int size = 4);
	int addUnnamed(int size = 4);

	void addToGlobal(const std::string& name, const std::string& value);

	int getStackPtrOffsetForVar(std::string name, int offset = 0, bool isBelow = false);
	int getStackPtrCountBackWards(int offset);

	int uniqueCompileNumber();
	Label getUniqueLabel();
	Scope createNewLabelScope();
	Scope createNewLabelScopeWithParentsTrue();
	Scope createNewLabelScopeWithParentsFalse();
	//std::tuple<std::string, std::string> scopeLabelNames();
	bool hasScopeLabels();
	std::string useFalseLabel();
	std::string useTrueLabel();
	std::string declareTrueLabel();
	std::string declareFalseLabel();
	bool usedFalseLabel();
	bool usedTrueLabel();

	std::string popScope();


};


