#include "ReservedReg.h"
#include "RegesterManager.h"


ReserveReg::ReserveReg(int& reservedReg) : reservedReg(reservedReg) {
	reserved = true;
}



bool ReserveReg::Reserved() {
	return reserved;
}

void ReserveReg::UnReserve() {
	reserved = false;
}



std::tuple<std::string, int> ReserveReg::wasRegReserved(const std::string& mutableInstructions) {
	if (!reserved) {
		auto newRegi = getFreeRegesterIndex();
		auto newReg = getFreeRegester(newRegi);
		//save left to stack
		auto instruction = std::string();

		int stackIndex = 1;// currentScope->variables.size() * 4;

		instruction += "push " + newReg + '\n';

		instruction += mutableInstructions;

		instruction += "pop " + newReg + '\n';


		return std::make_tuple(instruction, newRegi);
	}
	else {
		std::string s = mutableInstructions;
		return std::make_tuple(s, std::move(reservedReg));
	}

}
