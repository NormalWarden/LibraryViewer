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
	inline std::string_view FILENAME{ "userdata.txt" };
	inline constexpr size_t MAX_FAVORITE_BOOKS{ 5 };
	inline constexpr size_t MAX_RECENT_BOOKS{ 5 };
	enum class ResponseCode;
	enum class Sort;
	struct SearchParams;
	struct Book;

	long testConnection(); // Get status code after simple request to site
	ResponseCode recreateFile(std::string_view fileTemplate = JSONTemplates::startTemplate); // Creates the file if it doesn't exist or clears it
	ResponseCode fileToJSON(nlohmann::json& userdata); // Reads the file and writes it to nlohmann::json
	ResponseCode JSONToFile(const nlohmann::json& userdata); // Reads the nlohmann::json and writes it to file

	std::vector<std::string> getUsers(); // Gets all users from file
	ResponseCode createUser(const std::string& username); // Creates the user with the username in file
	ResponseCode chooseUser(std::string& username, const short choice); // Chooses user in username
	ResponseCode deleteUser(const std::string& username); // Deletes the user with the username in file

	std::vector<Book> getFavoriteBooks(std::string_view username); // Gets all user favorite books from file
	ResponseCode addFavoriteBook(std::string_view username, const Book& book); // Adds the user's favorite book to the file
	ResponseCode deleteFavoriteBook(std::string_view username, const short bookNumber); // Removes the user's favorite book from the file
	
	std::vector<Book> getRecentBooks(std::string_view username); // Gets all user recent books from file
	ResponseCode addRecentBook(std::string_view username, const Book& book); // Adds the user's recent book to the file
	ResponseCode deleteRecentBook(std::string_view username, const short bookNumber); // Removes the user's recent book from the file

	std::string getBookDescription(std::string_view link); // Request the book site and parse it to get description

	std::vector<Book> search(const SearchParams& sParams);
	Book randomSearch(const SearchParams& sParams); // Search without title, sortmode and resListSize

	ResponseCode getSearchParams(std::string_view username, SearchParams& sParams); // Gets user's search parameters from file
	ResponseCode saveSearchParams(std::string_view username, const SearchParams& sParams); // Writes user's search parameters to file

	// Functions-helpers
	inline int randBookNumber(unsigned int maxNum); // 0...maxNum
	inline ResponseCode isRandomBookEmpty(const Book& book);
	inline std::string transformStrToURL(std::string_view str); // "first second third" -> first+second+third
	inline std::string makeLinkFromResponse(const nlohmann::json& response, int bookNumber); // Site URL + book key in response + title
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
	size_t year{};
};

#include "engine.inl"