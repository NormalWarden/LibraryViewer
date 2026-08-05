#pragma once

#define CONNECTION_SUCCESS 0
#define CONNECTION_FAILURE 1
#define USER_NOT_SELECTED 0
#define USER_SELECTED 1
#define USERDATA_FILENAME "userdata.txt"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "book.h"
#include "language.h"
#include "jsonTemplates.h"

using json = nlohmann::json;

namespace Engine
{
	enum class Sort;
	struct SearchParams;
	bool testConnection(); // test request for response 200
	void createUser(json& userData);
	std::vector<Book> userBooks(std::string user);
	std::vector<Book> recentlyBooks(std::string user);
	std::vector<Book> search(const SearchParams& sParams);
	void printBooksList(const json& userData);
	void changeSearchParams(SearchParams& sParams);
	Book randomSearch(std::string author, std::vector<std::string> language, size_t year);
	bool chooseUser(std::string& user);
	void getSearchParams(const std::string& user, SearchParams& sParams);
	json fileToJSON();
}

enum class Engine::Sort
{
	None,
	Editions,
	Old,
	New,
	Rating
};

struct Engine::SearchParams
{
	std::string author;
	std::vector<std::string> langs;
	std::string title;
	size_t year{};
	Sort sort = Sort::None;
	size_t resListSize = 10;
};