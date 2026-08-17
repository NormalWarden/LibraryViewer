#pragma once

#define USER_NOT_SELECTED 0
#define USER_SELECTED 1

//#include <iostream>
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
	ResponseCode createUser(const std::string& username);
	ResponseCode deleteUser(const std::string& username);
	void chooseUser(std::string& username, const short choice);
	std::vector<std::string_view> getUsers(const std::string& filename = "userdata.txt");
	std::vector<Book> getFavoriteBooks(const std::string& username);
	std::vector<Book> getRecentlyBooks(const std::string username);
	std::vector<Book> search(const SearchParams& sParams);
	Book randomSearch(const SearchParams& sParams);
	ResponseCode getSearchParams(const std::string& username, SearchParams& sParams);
	ResponseCode saveSearchParams(const std::string& username, const SearchParams& sParams);
	ResponseCode addFavoriteBook(const std::string& username, Book book);
	ResponseCode deleteFavoriteBook(const std::string& username, const short bookNumber);
	ResponseCode addRecentlyBook(const std::string& username, Book book);
	ResponseCode deleteRecentlyBook(const std::string& username, const short bookNumber);
	json fileToJSON(const std::string& filename = "userdata.txt");
	ResponseCode JSONToFile(const json& userdata, const std::string& filename = "userdata.txt");
	ResponseCode recreateFile(const std::string& filename = "userdata.txt");
}

enum class Engine::ResponseCode
{
	Ok,
	EmptyJSON,
	NoUser,
	FailedFileUpdate,
	EmptyUsername,
	CreatingIdenticalUser
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