#pragma once
#include "RegexHelpers.h"
#include <regex>
#include <string_view>
#include <vector>
#include <tuple>

bool regex_search_string_view_once(std::string_view text, const std::regex& reg, std::string_view& ref) {

	std::vector<std::string_view> result;
	std::match_results<std::string_view::const_iterator> matches;
	std::regex_search(text.cbegin(), text.cend(), matches, reg);
	if (matches.size() > 0) {
		auto match = matches[0];
		auto first = match.first;
		auto last = match.second;
		auto size = static_cast<std::size_t>(last - first);
		auto start = static_cast<std::size_t>(first - text.cbegin());

		ref = text.substr(start, size);
		return true;
	}

	return false;
}

bool regex_string_same(std::string_view text, const std::regex& reg) {

	std::vector<std::string_view> result;
	std::match_results<std::string_view::const_iterator> matches;

	if (std::regex_search(text.cbegin(), text.cend(), matches, reg)) {
		if (matches.size() > 0) {
			auto match = matches[0];
			auto first = match.first;
			auto last = match.second;

			return  first == text.cbegin() && last == text.cend();
		}
		auto start = static_cast<std::size_t>(matches.suffix().first - text.cbegin());
		text = text.substr(start);
		return false;
	}

	return false;
}

std::tuple<size_t, size_t> regex_search_string_view_first(std::string_view text, const std::regex& reg) {

	std::vector<std::string_view> result;
	std::match_results<std::string_view::const_iterator> matches;
	std::regex_search(text.cbegin(), text.cend(), matches, reg);
	if (matches.size() > 0) {
		auto match = matches[0];

		auto start = static_cast<std::size_t>(match.first - text.cbegin());

		auto end = static_cast<std::size_t>(match.second - text.cbegin());
		return std::make_tuple(start, end);
	}

	return std::make_tuple(std::string_view::npos, std::string_view::npos);
}

bool isWhiteSpaceOnly(std::string_view text) {
	return regex_string_same(text, std::regex("\\s*"));
}

std::vector<std::string_view> regex_search_string_view(std::string_view text, const std::regex& reg) {

	std::vector<std::string_view> result;
	std::match_results<std::string_view::const_iterator> matches;

	while (std::regex_search(text.cbegin(), text.cend(), matches, reg)) {
		if (matches.size() > 0) {
			auto match = matches[0];
			auto first = match.first;
			auto last = match.second;
			auto size = static_cast<std::size_t>(last - first);
			auto start = static_cast<std::size_t>(first - text.cbegin());

			result.push_back(text.substr(start, size));
		}
		auto start = static_cast<std::size_t>(matches.suffix().first - text.cbegin());
		text = text.substr(start);
	}

	return result;
}

std::string_view functionName(std::string_view function) {
	return regex_search_string_view(function, std::regex("\\w+"))[1];
}

std::string_view variableName(std::string_view function) {
	return regex_search_string_view(function, std::regex("\\w+"))[1];
}
std::string_view variableType(std::string_view function) {
	return regex_search_string_view(function, std::regex("\\w+(\\s*\\*|)"))[0];
}

std::string functionType(std::string_view function, const std::vector<std::string_view>& paramTypes) {
	auto name = std::string(functionName(function));
	name += "_";
	for (auto param : paramTypes) {
		name += param;
		name += "_";
	}
	return name;
	
}