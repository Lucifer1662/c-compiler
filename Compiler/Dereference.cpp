#include "Dereference.h"
#include "Regester.h"

Dereference::Dereference(std::unique_ptr<Expression>&& left)
	: UnaryOperator(std::move(left)) {

}

std::tuple<std::string, std::shared_ptr<Memory>> Dereference::operator()(Scope& scope)
{
	auto [instructions, leftPtr] = left->operator()(scope);

	if (leftPtr->isDereference()) {
		auto reg = std::make_shared<Regester>(dataType);
		instructions += "mov " + reg->toString() + ", " + leftPtr->toString() + "\n";
		leftPtr = reg;

	}
	leftPtr->Dereference();

	return { instructions, leftPtr };
}

std::tuple<std::string, std::shared_ptr<Memory>> Dereference::operator()(Scope& scope, std::shared_ptr<Memory>& value)
{
	auto [instructions, leftPtr] = left->operator()(scope, value);

	if (leftPtr->isDereference()) {
		auto reg = std::make_shared<Regester>(dataType);
		instructions += "mov " + reg->toString() + ", " + leftPtr->toString();
		leftPtr = reg;
	}
	else {
		leftPtr->Dereference();
	}

	return { instructions, leftPtr };
}


