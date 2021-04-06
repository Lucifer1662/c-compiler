#include "ScopeContent.h"
#include "RegexHelpers.h"

auto& ScopeContent::createChild() {
	children.push_back(std::make_unique<ScopeContent>());
	children.back()->parent = this;
	return children.back();
}

std::tuple<int, std::tuple<size_t, size_t>, std::tuple<size_t, size_t>, std::regex> ScopeContent::getContext(std::string_view text, const std::vector<std::tuple<int, std::regex, std::regex>>& scopes) {
	auto closeReg = std::get<1>(scopes[0]);
	auto open = std::make_tuple(std::string_view::npos, std::string_view::npos);
	int type = 0;
	for (auto& [_type, left, right] : scopes)
	{
		auto open1 = regex_search_string_view_first(text, left);
		if (std::get<0>(open) > std::get<0>(open1)) {
			open = open1;
			closeReg = right;
			type = _type;
		}
	}
	auto close = regex_search_string_view_first(text, closeReg);
	return std::make_tuple(type, open, close, closeReg);
}

std::tuple<int, size_t, size_t> ScopeContent::getContextFlat(std::string_view text, const std::vector<std::tuple<int, std::regex>>& patturns) {

	auto open = std::make_tuple(std::string_view::npos, std::string_view::npos);
	int type;
	for (auto& [_type, patturn] : patturns)
	{
		auto open1 = regex_search_string_view_first(text, patturn);
		if (std::get<0>(open) > std::get<0>(open1)) {
			open = open1;
			type = _type;
		}
	}
	return std::make_tuple(type, std::get<0>(open), std::get<1>(open));
}

void ScopeContent::popChildIfEmpty() {
	auto& child = children.back();
	if (child->children.size() == 0 &&
		isWhiteSpaceOnly(child->context))
		children.pop_back();
}

void ScopeContent::RemoveBackIfRedudantNodes() {

	if (isWhiteSpaceOnly(children.back()->context)) {
		//move my children to my parents children
		auto child = std::move(children.back());
		children.pop_back();
		for (auto& child : child->children)
		{
			children.push_back(std::move(child));
			children.back()->parent = this;
		}
	}
}

void ScopeContent::MoveToParentIfRedudantNodes(int i) {
	if (isWhiteSpaceOnly(children[i]->context)) {
		//move my children to my parents children
		int childI = i;
		for (auto& child : children[i]->children)
		{
			child->parent = this;
		}

		children.insert(children.begin() + i + 1,
			std::make_move_iterator(children[i]->children.begin()),
			std::make_move_iterator(children[i]->children.end()));
		children.erase(children.begin() + i);

	}
}

size_t ScopeContent::parseText(const std::vector<std::tuple<int, std::regex, std::regex>>& scopes, std::regex* ender) {
	auto offset = 0;
	do {
		auto [_type, open, close, closer] = getContext(context, scopes);


		auto [openIndexStart, openIndexEnd] = open;


		if (ender) {
			auto [enderStart, enderEnd] = regex_search_string_view_first(context, *ender);
			if (enderStart < openIndexStart) {
				context = context.substr(0, enderStart);
				break;
			}
		}

		if (openIndexStart == std::string_view::npos) {
			break;
		}
		//handle none matched string at the front
		{
			auto& child = createChild();
			child->context = context.substr(0, openIndexStart);
			child->type = -1;
			child->parseText(scopes);
			RemoveBackIfRedudantNodes();


			context = context.substr(openIndexStart);
			offset += openIndexStart;
			openIndexEnd -= openIndexStart;
			openIndexStart = 0;
		}

		//handle the matched string
		{
			auto& child = createChild();
			child->context = context.substr(0, openIndexEnd);
			child->type = _type;

			auto& child_child = child->createChild();
			child_child->context = context.substr(openIndexEnd);
			auto closeIndexEnd = child_child->parseText(scopes, &closer);
			child->RemoveBackIfRedudantNodes();

			try {
				context = context.substr(openIndexEnd + closeIndexEnd);
				offset += openIndexEnd + closeIndexEnd;
			}
			catch (std::exception e) {
				int y = 0;
			}

			/*if (openIndexEnd + closeIndexEnd > context.size()) {
			context = context.substr(context.size());
			offset += openIndexEnd + context.size();
			}
			else {
			context = context.substr(openIndexEnd + closeIndexEnd);
			offset += openIndexEnd + closeIndexEnd;
			}*/
		}

	} while (context.size() > 0);


	return offset + context.size() + 1;
}

std::tuple<int, int> ScopeContent::findFirstNextType(const std::vector<std::tuple<int, int>>& types, int offset) {
	for (int i = offset; i < children.size(); i++)
	{
		for (size_t j = 0; j < types.size(); j++)
		{
			if (children[i]->type == std::get<0>(types[j]))
				return std::make_tuple(std::get<1>(types[j]), i);
		}
	}
	return std::make_tuple(-1, -1);
}

int ScopeContent::findFirstNextType(const int ender, int offset) {
	for (int i = offset; i < children.size(); i++)
	{
		if (children[i]->type == ender)
			return i;

	}
	return -1;
}

size_t ScopeContent::parseTextV2(const std::vector<std::tuple<int, int>>& types, int* ender, int offset) {
	auto openIndex = 0;
	do {
		auto [closer, openIndex] = findFirstNextType(types, offset);



		if (ender) {
			auto end = findFirstNextType(*ender, offset);
			if (end < openIndex || openIndex == -1) {
				return end;
			}
		}

		if (openIndex == -1) {
			break;
		}



		{
			auto& openerChild = children[openIndex];

			auto endIndex = parseTextV2(types, &closer, openIndex + 1);

			openerChild->children.insert(openerChild->children.begin(),
				std::make_move_iterator(children.begin() + openIndex + 1),
				std::make_move_iterator(children.begin() + endIndex));

			children.erase(children.begin() + openIndex + 1, children.begin() + endIndex + 1);
			offset = openIndex + 1;
		}

	} while (openIndex != -1);


	return offset;
}

void ScopeContent::parseTextFlat(const std::vector<std::tuple<int, std::regex>>& patturns) {
	do {
		auto [_type, start, end] = getContextFlat(context, patturns);
		if (start == std::string_view::npos)
			break;

		auto top = context.substr(0, start);
		auto mid = context.substr(start, end - start);
		auto back = context.substr(end);
		if (top != "") {
			createChild()->context = top;
			RemoveBackIfRedudantNodes();
		}
		auto& midChild = createChild();
		midChild->context = mid;
		midChild->type = _type;
		RemoveBackIfRedudantNodes();

		context = back;

	} while (context.size() > 0);
}


void ScopeContent::parseTextBinop(const std::vector<std::tuple<int, std::regex>>& patturns) {

	for (size_t i = 0; i < children.size(); i++)
	{

		auto [_type, start, end] = getContextFlat(children[i]->context, patturns);

		if (start != std::string_view::npos) {

			children[i]->type = _type;
			children[i]->children.push_back(std::move(children[i - 1]));
			children[i]->children.push_back(std::move(children[i + 1]));
			children.erase(children.begin() + i + 1);
			children.erase(children.begin() + i - 1);
		}
	}

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->parseTextBinop(patturns);
	}

}

void ScopeContent::parseTextUnary(const std::vector<std::tuple<int, std::regex>>& patturns) {

	if (type == -1) {
		auto [_type, start, end] = getContextFlat(context, patturns);

		if (start != std::string_view::npos) {
			auto top = context.substr(0, start);
			auto mid = context.substr(start, end - start);
			auto back = context.substr(end);
			if (back != "" || top != "") {
				if (top != "") {
					createChild()->context = top;
					RemoveBackIfRedudantNodes();
				}
				auto& midChild = createChild();
				midChild->context = mid;
				midChild->type = _type;
				RemoveBackIfRedudantNodes();

				if (back != "") {
					midChild->createChild()->context = back;
					midChild->RemoveBackIfRedudantNodes();
				}
				context = context.substr(context.size());
				//auto index = getContextFlat(context, patturns);
				//_type = std::get<0>(index);
				//start = std::get<1>(index);
				//end = std::get<2>(index);
			}

		}
	}
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->parseTextUnary(patturns);
		MoveToParentIfRedudantNodes(i);
	}



}

void ScopeContent::makeAdjacenetChildren(const std::tuple<int, int, int>& tup) {
	auto [type, left, right] = tup;
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]->type == type && i - left >= 0 && i + right < children.size()) {
			if (right > 0) {
				children[i]->children.insert(
					children[i]->children.begin(),
					std::make_move_iterator(children.begin() + i + 1),
					std::make_move_iterator(children.begin() + i + 1 + right));

				children.erase(children.begin() + i + 1, children.begin() + i + 1 + right);
			}
			if (left > 0) {
				children[i]->children.insert(
					children[i]->children.begin(),
					std::make_move_iterator(children.begin() + i - left),
					std::make_move_iterator(children.begin() + i));

				children.erase(children.begin() + i - left, children.begin() + i);
				i -= left;
			}
		}

	}

	for (auto& child : children)
	{
		child->makeAdjacenetChildren(tup);
	}
}



void ScopeContent::removeUnclassifiedLeafs() {
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->children.size() == 0 && children[i]->type == -1) {
			children.erase(children.begin() + i);
			i--;
		}
		else {
			children[i]->removeUnclassifiedLeafs();
		}


	}

}
