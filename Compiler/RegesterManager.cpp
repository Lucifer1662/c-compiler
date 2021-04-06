#include "RegesterManager.h"
#include "ReservedReg.h"

std::vector<std::weak_ptr<ReserveReg>> reservedReg = std::vector<std::weak_ptr<ReserveReg>>(3);

const std::vector<std::string> generalRegesters = {
	"eax", "ebx", "ecx"
};

void makeRegesterFree(int reg) {
	if (!reservedReg[reg].expired())
		reservedReg[reg].lock()->UnReserve();
}

int getFreeRegesterIndex() {
	size_t i;
	for (i = 0; i < reservedReg.size(); i++)
	{
		if (reservedReg[i].expired()) {
			return i;
		}
	}
	//else swap
	i = rand() % reservedReg.size();

	makeRegesterFree(i);

	return i;
}

std::string getFreeRegester(int regi) {
	return generalRegesters[regi];
}

std::shared_ptr<ReserveReg> reserveReg(int reg) {
	auto newReserveReg = std::make_shared<ReserveReg>(reg);
	if (!reservedReg[reg].expired()) {
		//kick out the old
		reservedReg[reg].lock()->UnReserve();
	}
	reservedReg[reg] = newReserveReg;
	return newReserveReg;
}
