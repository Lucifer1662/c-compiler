#pragma once
#include <string_view>
#include <vector>
#include <regex>
#include <tuple>





struct ScopeContent {
	ScopeContent* parent;
	std::vector<std::unique_ptr<ScopeContent>> children;
	std::string_view context;

	int type = -1;


	auto& createChild();

	std::tuple<int, std::tuple<size_t, size_t>, std::tuple<size_t, size_t>, std::regex>
		getContext(std::string_view text, const std::vector<std::tuple<int, std::regex, std::regex>>& scopes);


	std::tuple<int, size_t, size_t> getContextFlat(std::string_view text, const std::vector<std::tuple<int, std::regex>>& patturns);

	void popChildIfEmpty();

	void RemoveBackIfRedudantNodes();

	void MoveToParentIfRedudantNodes(int i);

	size_t parseText(const std::vector<std::tuple<int, std::regex, std::regex>>& scopes, std::regex* ender = nullptr);

	std::tuple<int, int> findFirstNextType(const std::vector<std::tuple<int, int>>& types, int offset = 0);

	int findFirstNextType(const int ender, int offset = 0);

	size_t parseTextV2(const std::vector<std::tuple<int, int>>& types, int* ender = nullptr, int offset = 0);

	void parseTextFlat(const std::vector<std::tuple<int, std::regex>>& patturns);

	void parseTextBinop(const std::vector<std::tuple<int, std::regex>>& patturns);

	void parseTextUnary(const std::vector<std::tuple<int, std::regex>>& patturns);

	void makeAdjacenetChildren(const std::tuple<int, int, int>& tup);

	template<typename ComparatorLeft, typename ComparatorRight>
	void resolveAmbiguousSymbols(int matchSymbol, int newSymbol, 
		const ComparatorLeft& left, const ComparatorRight right ) {

		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->type == matchSymbol) {
				
				if (right(children.begin() + i + 1, children.end()) || 
					left(children.rbegin() + children.size() - i, children.rend())
					) {
					//change
					children[i]->type = newSymbol;
				}
			}

		}

		for (auto& child : children)
		{
			child->resolveAmbiguousSymbols(matchSymbol, newSymbol, left, right);
		}
	
	}

	template<typename ComparatorLeft, typename ComparatorRight>
	void makeAdjacenetChildrenFunc(int type, const ComparatorLeft& left, const ComparatorRight& right) {


		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->type == type) {
				auto rightCount = right(children.begin() + i + 1, children.end());
				if (rightCount) {
					children[i]->children.insert(
						children[i]->children.begin(),
						std::make_move_iterator(children.begin() + i + 1),
						std::make_move_iterator(children.begin() + i + 1 + rightCount));

					children.erase(children.begin() + i + 1, children.begin() + i + 1 + rightCount);
				}

				auto leftCount = left(children.rbegin() + children.size() - i -1, children.rend());
				if (leftCount) {
					children[i]->children.insert(
						children[i]->children.begin(),
						std::make_move_iterator(children.begin() + i - leftCount),
						std::make_move_iterator(children.begin() + i));

					children.erase(children.begin() + i - leftCount, children.begin() + i);
					i -= leftCount;
				}
			}

		}

		for (auto& child : children)
		{
			child->makeAdjacenetChildrenFunc(type, left, right);
		}
	}



	void removeUnclassifiedLeafs();
};



