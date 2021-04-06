#include "ConstantFloatValue.h"
#include "Stack.h"

ConstantFloatValue::ConstantFloatValue(std::string name, std::string value, std::shared_ptr<Type> dataType)
	:GlobalVariable(name, value, GlobalVariable::Size::Byte4,  dataType)
{

}

std::tuple<std::string, std::shared_ptr<Memory>> ConstantFloatValue::operator()(Scope& scope)
{
	auto [instructions, value] = GlobalVariable::operator()(scope);
	//auto reg = std::make_unique<Regester>(dataType);
	//instructions += "mov " + reg->toString() + ", [" + value->toString() + "]\n";
	//return std::make_tuple(instructions, std::move(reg));


	//auto instructions = name;
	//instructions += " " + sizeNames[(int)size] + " " + value + "\n";

	//scope.addToGlobal(name, instructions);

	return std::make_tuple("", std::make_unique<NameAddress>('[' + name + ']', dataType));
}
