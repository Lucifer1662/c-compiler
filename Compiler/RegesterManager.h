#pragma once
#include <vector>
#include <memory>
#include <string>
#include "ReservedReg.h"

void makeRegesterFree(int reg);

int getFreeRegesterIndex();


std::string getFreeRegester(int regi);

std::shared_ptr<ReserveReg> reserveReg(int reg);
