#include "Scope.h"
#include <string>
#include <tuple>

Scope::Scope() : globalNames(std::make_shared<std::map<std::string, std::string>>()),
globalCompileNameCount(std::make_shared<int>(0)),
type(ScopeEnum::Global),
parent(nullptr),
floatingStack(FloatingStackManager::make_FloatStackManager())
{}

Scope::Scope(Scope* parent) :
	globalNames(parent->globalNames),
	parent(parent),
	globalCompileNameCount(parent->globalCompileNameCount),
	type(parent->type),
	floatingStack(parent->floatingStack)
{}

Scope Scope::childScope() {
	return Scope(this);
}

Scope Scope::createFunctionScope() {
	auto child = childScope();
	child.type = ScopeEnum::FunctionCall;
	return child;
}


int Scope::addName(std::string name, int size) {
	names.emplace_back(name, size);
	this->size += size;
	return -this->size;
}

int Scope::addUnnamed(int size)
{
	names.emplace_back(size);
	this->size += size;
	return -this->size;
}

void Scope::addToGlobal(const std::string& name, const std::string& value) {
	globalNames->insert_or_assign(name, value);
	//global->append(value);
}

int Scope::getStackPtrOffsetForVar(std::string name, int offset, bool isBelow) {
	for (int i = names.size() - 1; i >= 0; i--)
	{
		if (names[i].name == name) {
			if (isBelow)
				return offset + 4;
			else
				return - (size - offset);
		}
		offset += names[i].size;
	}
	if (parent) {
		auto parentName = parent->getStackPtrOffsetForVar(name, offset, parent->type == ScopeEnum::FunctionCall);
		return parentName;
	}
	return 0;
}

int Scope::getStackPtrCountBackWards(int offset)
{
	int count = 0;
	for (size_t i = 0; i < names.size(); i++)
	{
		count -= names[i].size;

		if (count == offset) {
			return names.size()-i;
		}

		
	}
	return -1;
}


int Scope::uniqueCompileNumber()
{
	return (*globalCompileNameCount)++;
}

Label Scope::getUniqueLabel()
{
	return Label(uniqueCompileNumber());
}


Scope Scope::createNewLabelScope() {
	auto child = childScope();
	child.type = ScopeEnum::Label;
	child.labels = ScopeLabels(uniqueCompileNumber(), uniqueCompileNumber());
	return child;
}

Scope Scope::createNewLabelScopeWithParentsTrue() {
	auto child = childScope();
	child.type = ScopeEnum::Label;
	child.labels = ScopeLabels(labels->trueLabel, uniqueCompileNumber());
	return child;
}

Scope Scope::createNewLabelScopeWithParentsFalse()
{
	auto child = childScope();
	child.type = ScopeEnum::Label;
	child.labels = ScopeLabels(uniqueCompileNumber(), labels->falseLabel);
	return child;
}

//std::tuple<std::string, std::string> Scope::scopeLabelNames() {
//	if(labels.has_value())
//		return std::make_tuple(labels.value().trueLabel->uniqueCompileName(), labels.value().falseLabel->uniqueCompileName());
//	return std::make_tuple("", "");
//}


bool Scope::hasScopeLabels()
{
	return labels.has_value();
}

std::string Scope::useFalseLabel()
{
	return labels->falseLabel->use();
}

std::string Scope::useTrueLabel()
{
	return  labels->trueLabel->use();
}

std::string Scope::declareTrueLabel()
{
	return labels->trueLabel->declareLabel();
}

std::string Scope::declareFalseLabel()
{
	return labels->falseLabel->declareLabel();
}

bool Scope::usedFalseLabel()
{
	return labels->falseLabel->hasUsed();
}

bool Scope::usedTrueLabel()
{
	return labels->falseLabel->hasUsed();
}

std::string Scope::popScope()
{
	return 	"add esp, " + std::to_string(size) + '\n';
}

