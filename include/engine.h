#pragma once

#define USER_NOT_SELECTED 0
#define USER_SELECTED 1
#define USERDATA_FILENAME "test.txt"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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
	enum class Sort;
	struct SearchParams;
	long testConnection(); // test request for response 200
	bool createUser(json& userData);
	std::vector<Book> favoriteBooks(const std::string& user);
	std::vector<Book> recentlyBooks(std::string user);
	std::vector<Book> search(const SearchParams& sParams);
	void printBooksList(const std::vector<Book>& books);
	bool changeSearchParams(const std::string& user, SearchParams& sParams);
	Book randomSearch();
	bool chooseUser(std::string& user);
	void getSearchParams(const std::string& user, SearchParams& sParams);
	json fileToJSON();
	void printSearchParams(const SearchParams& sParams);
	void changeAuthorSearch(SearchParams& sParams);
	void changeLanguageSearch(SearchParams& sParams);
	void changeTitleSearch(SearchParams& sParams);
	void changeYearSearch(SearchParams& sParams);
	void changeSortSearch(SearchParams& sParams);
	void changeResListSizeSearch(SearchParams& sParams);
	bool saveSearchParams(const std::string& user, const SearchParams& sParams);
	bool addFavoriteBook(const std::string& user, Book book);
	bool addRecentlyBook(const std::string& user, Book book);
	bool JSONToFile(const json& userData);
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