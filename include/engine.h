#pragma once

#define USER_NOT_SELECTED 0
#define USER_SELECTED 1

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
	inline std::string filename{ "userdata.txt" };
	enum class ResponseCode;
	enum class Sort;
	struct SearchParams;

	long testConnection();
	ResponseCode recreateFile(std::string_view fileTemplate = JSONTemplates::startTemplate);
	ResponseCode fileToJSON(json& userdata);
	ResponseCode JSONToFile(const json& userdata);

	std::vector<std::string> getUsers();
	ResponseCode createUser(std::string& username);
	ResponseCode chooseUser(std::string& username, const short choice);
	ResponseCode deleteUser(std::string& username);

	std::vector<Book> getFavoriteBooks(std::string_view username);
	ResponseCode addFavoriteBook(std::string_view username, Book book);
	ResponseCode deleteFavoriteBook(std::string_view username, const short bookNumber);
	
	std::vector<Book> getRecentlyBooks(std::string_view username);
	ResponseCode addRecentlyBook(std::string_view username, Book book);
	ResponseCode deleteRecentlyBook(std::string_view username, const short bookNumber);

	std::string getBookDescription(std::string_view link);

	std::vector<Book> search(const SearchParams& sParams);
	Book randomSearch(const SearchParams& sParams);

	ResponseCode getSearchParams(std::string_view username, SearchParams& sParams);
	ResponseCode saveSearchParams(std::string_view username, const SearchParams& sParams);

	int randBookNumber(unsigned int maxNum);
	ResponseCode isRandomBookEmpty(Book book);
}

enum class Engine::ResponseCode
{
	Ok,
	EmptyJSON,
	NoUser,
	FailedFileUpdate,
	FailedFileOpen,
	EmptyUsername,
	CreatingIdenticalUser,
	InvalidInput,
	GuestFavoriteOrRecentlyBook,
	EmptyRandomBook,
	MaxCountOfFavoriteBooks
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
	std::vector<std::string> authors;
	std::vector<std::string> langs;
	std::string title;
	size_t year{};
	Sort sort = Sort::None;
	size_t resListSize = 10;
};