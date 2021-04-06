#include "BinaryOperator.h"
#include <algorithm>


BinaryOperator::BinaryOperator(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) :
	left(std::move(left)), right(std::move(right)), Expression(
		left->dataType->id > right->dataType->id ? left->dataType : right->dataType
	) {

	
}
