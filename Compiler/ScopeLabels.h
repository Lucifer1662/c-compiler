#pragma once
#include "Label.h"
#include <memory>

struct ScopeLabels
{
	std::shared_ptr<Label> trueLabel;
	std::shared_ptr<Label> falseLabel;
	
	ScopeLabels(int trueLabel, int falseLabel) : 
		trueLabel(std::make_shared<Label>(trueLabel)), 
		falseLabel(std::make_shared<Label>(falseLabel)) {}

	ScopeLabels(std::shared_ptr<Label> trueLabel, int falseLabel) :
		trueLabel(trueLabel),
		falseLabel(std::make_shared<Label>(falseLabel)) {}

	ScopeLabels(int trueLabel, std::shared_ptr<Label> falseLabel) :
		trueLabel(std::make_shared<Label>(trueLabel)),
		falseLabel(falseLabel) {}

	ScopeLabels(std::shared_ptr<Label> trueLabel, std::shared_ptr<Label> falseLabel) :
		trueLabel(trueLabel),
		falseLabel(falseLabel) {}

};

