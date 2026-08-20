#pragma once
#include <iostream>
#include <vector>
#include <string_view>
#include "engine.h"
#include "book.h"

namespace UI
{
	void firstMessage();

	void printOptions();
	void printSpecialOptions();
	short getUserChoice();
	std::string getNewUsername();

	void printChosenUser(std::string_view username);
	void printUsers(const std::vector<std::string_view>& users);
	void printBooks(const std::vector<Book>& books);
	void systemMessage(const Engine::ResponseCode response);

	void changeAuthorSearch(Engine::SearchParams& sParams);
	void changeLanguageSearch(Engine::SearchParams& sParams);
	void changeTitleSearch(Engine::SearchParams& sParams);
	void changeYearSearch(Engine::SearchParams& sParams);
	void changeSortSearch(Engine::SearchParams& sParams);
	void changeResListSizeSearch(Engine::SearchParams& sParams);

	void changeSearchParams(const std::string& username, Engine::SearchParams& sParams);
	void printSearchParams(const Engine::SearchParams& sParams);

	Engine::SearchParams specifyingRandomSearchParams();

	void printTestConnectionRes(const long status);
}