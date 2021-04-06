#pragma once
#include <tuple>
#include <string>


struct ReserveReg {
	bool reserved = false;
	int reservedReg;
	ReserveReg(int& reservedReg);

	bool Reserved();

	void UnReserve();

	std::tuple<std::string, int> wasRegReserved(const std::string& mutableInstructions);

};


