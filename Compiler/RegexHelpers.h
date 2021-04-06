#pragma once
#include <regex>
#include <string_view>
#include <vector>
#include <tuple>

bool regex_search_string_view_once(std::string_view text, const std::regex& reg, std::string_view& ref);

bool regex_string_same(std::string_view text, const std::regex& reg);

std::tuple<size_t, size_t> regex_search_string_view_first(std::string_view text, const std::regex& reg);

bool isWhiteSpaceOnly(std::string_view text);

std::vector<std::string_view> regex_search_string_view(std::string_view text, const std::regex& reg);

std::string_view functionName(std::string_view function);
std::string_view variableName(std::string_view function);
std::string_view variableType(std::string_view function);
std::string functionType(std::string_view function, const std::vector<std::string_view>& paramTypes);