#pragma once
#include <iostream>
#include <vector>
#include <string_view>
#include "engine.h"

namespace UI
{
	void firstMessage();

	void printOptions();
	void printSpecialOptions();
	void printAfterSearchOptions();
	short getUserChoice();
	std::string getNewUsername();
	void printChosenUser(std::string_view username);
	void printUsers(const std::vector<std::string>& users);
	void printBooks(const std::vector<Engine::Book>& books);
	void printBookDescription(std::string_view description);
	void printTestConnectionRes(const long status);
	void systemMessage(const Engine::ResponseCode response);

	void changeAuthorSearch(Engine::SearchParams& sParams);
	void changeLanguageSearch(Engine::SearchParams& sParams);
	void changeTitleSearch(Engine::SearchParams& sParams);
	void changeYearSearch(Engine::SearchParams& sParams);
	void changeSortSearch(Engine::SearchParams& sParams);
	void changeResListSizeSearch(Engine::SearchParams& sParams);

	void changeSearchParams(const std::string& username, Engine::SearchParams& sParams);
	void printSearchParams(const Engine::SearchParams& sParams);
	void printRandomSearchParams(const Engine::SearchParams& sParams);

	Engine::SearchParams specifyingRandomSearchParams();

	void lookBookDescriptionFromSearch(std::string_view username, const std::vector<Engine::Book>& books);
	void favoriteBookFromSearch(std::string_view username, const std::vector<Engine::Book>& books);
	void favoriteRandomBook(std::string_view username, const Engine::Book& book);

	// Functions-helpers
	inline std::string authorsVecToStr(const std::vector<std::string>& authorsV); // { "Mark Twain", "Charles Neider" } -> "Mark Twain, Charles Neider"
	inline std::string languagesVecToStr(const std::vector<std::string>& langsV); // { "english", "spanish" } -> "english, spanish"
	inline std::string sortModeToStr(const Engine::Sort& sort); // Engine::Sort::Mode -> "mode"
}

#include "ui.inl"