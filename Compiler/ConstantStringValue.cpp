#include "ConstantStringValue.h"


int getEncoded(char c) {
	switch (c) {
	case 'n':
		return (int)'\n';
	case 't':
		return (int)'\t';
	}
}

std::string parseToAssemblyString(const std::string& text) {
	auto newText = std::string();
	
	bool nextIsEscaped = false;
	bool isInStringFormat = false;
	
	for (auto& c : text) {
		
		if (c == '\\') {
			nextIsEscaped = true;
			continue;
		}else if (nextIsEscaped) {
			if (isInStringFormat)
				newText += '\'';
			newText += std::string(",") + std::to_string(getEncoded(c));
			nextIsEscaped = false;
			isInStringFormat = false;
		} else {
			if (!isInStringFormat) {
				newText += '\'';
				isInStringFormat = true;
			}
			newText += c;
		}
	}
	
	if (isInStringFormat) {
		newText += '\'';
	}

	newText += ",0";
	return newText;
}

ConstantStringValue::ConstantStringValue(std::string name, std::string value, std::shared_ptr<Type> dataType) :
	GlobalVariable(name,  parseToAssemblyString(value), GlobalVariable::Size::Byte, dataType) {}
