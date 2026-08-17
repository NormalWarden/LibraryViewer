#include <iostream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "engine.h"
#include "ui.h"

using json = nlohmann::json;

int main()
{
	UI::firstMessage();
	/*cpr::Response r = cpr::Get(cpr::Url{"https://openlibrary.org/search.json"},
		cpr::Parameters{ {"q", "the+lord+of+the+rings"} });
	json j = json::parse(r.text);*/
	//std::cout << j["docs"][0].dump(4);

	UI::printTestConnectionRes(Engine::testConnection());
	
	std::string user;
	Engine::SearchParams sParams;
	while (true)
	{
		UI::printOptions();
		switch (UI::getUserChoice())
		{
		case 1:
			UI::printBooks(Engine::getFavoriteBooks(user));
			break;
		case 2:
			UI::printBooks(Engine::getRecentlyBooks(user));
			break;
		case 3:
			UI::systemMessage(Engine::getSearchParams(user, sParams));
			UI::printSearchParams(sParams);
			UI::changeSearchParams(user, sParams);
			UI::systemMessage(Engine::saveSearchParams(user, sParams));
			UI::printBooks(Engine::search(sParams));
			break;
			// TODO: create option to look book in more details
		case 4:
			Engine::randomSearch(UI::specifyingRandomSearchParams());
			break;
			// TODO: create option to look book in more details
		case 5:
			UI::printChosenUser(user);
			UI::printUsers(Engine::getUsers());
			Engine::chooseUser(user, UI::getUserChoice());
			break;
		case 6:
			Engine::createUser(UI::getNewUsername());
		case 7:
			UI::printSpecialOptions();
			switch (UI::getUserChoice())
			{
			case 1:
				Engine::recreateFile();
				break;
			case 2:
				Engine::deleteUser(user);
				break;
			case 3:
				UI::printBooks(Engine::getFavoriteBooks(user));
				Engine::deleteFavoriteBook(user, UI::getUserChoice());
				break;
			case 4:
				UI::printTestConnectionRes(Engine::testConnection());
				break;
			case 5:
				break;
			}
		case 8:
			return 0;
		}
	}
}
