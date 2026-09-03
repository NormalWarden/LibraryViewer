#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include <random>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "language.h"
#include "jsonTemplates.h"


namespace Engine
{
	inline std::string filename{ "userdata.txt" };
	enum class ResponseCode;
	enum class Sort;
	struct SearchParams;
	struct Book;

	long testConnection();
	ResponseCode recreateFile(std::string_view fileTemplate = JSONTemplates::startTemplate);
	ResponseCode fileToJSON(nlohmann::json& userdata);
	ResponseCode JSONToFile(const nlohmann::json& userdata);

	std::vector<std::string> getUsers();
	ResponseCode createUser(const std::string& username);
	ResponseCode chooseUser(std::string& username, const short choice);
	ResponseCode deleteUser(std::string& username);

	std::vector<Book> getFavoriteBooks(std::string_view username);
	ResponseCode addFavoriteBook(std::string_view username, const Book& book);
	ResponseCode deleteFavoriteBook(std::string_view username, const short bookNumber);
	
	std::vector<Book> getRecentlyBooks(std::string_view username);
	ResponseCode addRecentlyBook(std::string_view username, const Book& book);
	ResponseCode deleteRecentlyBook(std::string_view username, const short bookNumber);

	std::string getBookDescription(std::string_view link);

	std::vector<Book> search(const SearchParams& sParams);
	Book randomSearch(const SearchParams& sParams);

	ResponseCode getSearchParams(std::string_view username, SearchParams& sParams);
	ResponseCode saveSearchParams(std::string_view username, const SearchParams& sParams);

	// Functions-helpers
	int randBookNumber(unsigned int maxNum);
	ResponseCode isRandomBookEmpty(const Book& book);
	std::string	transformStrToURL(std::string_view str);
	std::string	makeLinkFromResponse(const nlohmann::json& response, int bookNumber);
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

struct Engine::Book
{
	std::vector<std::string> author;
	std::vector<std::string> language;
	std::string title;
	std::string link;
	size_t year;
};