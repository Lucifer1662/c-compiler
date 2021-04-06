#include "ConstantValue.h"

ConstantValue::ConstantValue(std::string value, std::shared_ptr<Type> dataType) 
	: value(value),Expression(dataType)  {
	type = ExpressionEnum::Constant;
}


std::tuple<std::string, std::shared_ptr<Memory>> ConstantValue::operator()(Scope& scope) {

	//assert(left->op->index() == 2);
	auto reg = std::make_unique<Regester>(dataType);


	//get the regest the right hand side is in
	auto mov = "mov " + reg->toString() + ", " + value + "\n";

	return std::make_tuple(mov, std::move(reg));
}
