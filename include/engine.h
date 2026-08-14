#pragma once

#define USER_NOT_SELECTED 0
#define USER_SELECTED 1

#include <iostream>
#include <fstream>
#include <vector>
#include <string> // TODO: replace to string_view
#include <string_view>
#include <algorithm>
#include <random>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "book.h"
#include "language.h"
#include "jsonTemplates.h"

using json = nlohmann::json;

namespace Engine
{
	enum class ResponseCode;
	enum class Sort;
	struct SearchParams;

	long testConnection();
	bool createUser(const std::string& username, json& userdata);
	bool chooseUser(std::string& username);
	std::vector<std::string> getUsers(const std::string& filename = "userdata.txt");
	std::vector<Book> favoriteBooks(const std::string& username);
	std::vector<Book> recentlyBooks(const std::string username);
	std::vector<Book> search(const SearchParams& sParams);
	Book randomSearch();
	bool changeSearchParams(const std::string& username, SearchParams& sParams);
	ResponseCode getSearchParams(const std::string& username, SearchParams& sParams);
	void printSearchParams(const SearchParams& sParams);
	bool saveSearchParams(const std::string& username, const SearchParams& sParams);
	void changeAuthorSearch(SearchParams& sParams);
	void changeLanguageSearch(SearchParams& sParams);
	void changeTitleSearch(SearchParams& sParams);
	void changeYearSearch(SearchParams& sParams);
	void changeSortSearch(SearchParams& sParams);
	void changeResListSizeSearch(SearchParams& sParams);
	bool addFavoriteBook(const std::string& username, Book book);
	bool addRecentlyBook(const std::string& username, Book book);
	json fileToJSON(const std::string& filename = "userdata.txt");
	bool JSONToFile(const json& userdata, const std::string& filename = "userdata.txt");
}

enum class Engine::ResponseCode
{
	Ok,
	EmptyJSON,
	NoUser
};

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