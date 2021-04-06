#include "Minus.h"

std::tuple<std::string, std::shared_ptr<Memory>> Minus::operator()(Scope& scope) {
	return std::make_tuple("", nullptr);
}