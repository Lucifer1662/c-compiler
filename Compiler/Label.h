#pragma once
#include <string>
struct Label
{
	int number;
	bool used = false;
	Label(int number) :number(number) {}
	

	std::string uniqueCompileName()
	{
		return std::string("_CN") + std::to_string(number);
	}	

	bool hasUsed()
	{
		return used;
	}


	std::string use()
	{
		used = true;
		return uniqueCompileName();
	}

	std::string declareLabel() {
		if (used)
			return uniqueCompileName() + ":\n";
		else
			return "";
	}
};

