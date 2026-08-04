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

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "book.h"
#include "language.h"
#include "jsonTemplates.h"

using json = nlohmann::json;

namespace Engine
{
	enum class Sort;
	enum class YearSearch;
	struct SearchParams;
	bool testConnection(); // test request for response 200
	void createUser(json& userData);
	std::vector<Book> userBooks(std::string user);
	void printUsers();
	void chooseUser(std::string choosedUser, std::string& user);
	std::vector<Book> recentlyBooks(std::string user);
	void printBooksList(std::vector<Book> books);
	void changeSearchParams(SearchParams& params);
	Book randomSearch(std::string author, std::vector<std::string> language, size_t year);
	bool selectUser(std::string& user);
}