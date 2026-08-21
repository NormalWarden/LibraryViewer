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
		case 1: // Open list of my books
			UI::printBooks(Engine::getFavoriteBooks(user));
			break;
		case 2: // Look recently looked books
			UI::printBooks(Engine::getRecentlyBooks(user));
			break;
		case 3: // Search
			UI::systemMessage(Engine::getSearchParams(user, sParams));
			UI::printSearchParams(sParams);
			UI::changeSearchParams(user, sParams);
			UI::systemMessage(Engine::saveSearchParams(user, sParams));
			UI::printBooks(Engine::search(sParams));
			break;
			// TODO: create option to look book in more details
		case 4: // Search randomly
			Engine::randomSearch(UI::specifyingRandomSearchParams());
			break;
			// TODO: create option to look book in more details
		case 5: // Choose user
			UI::printChosenUser(user);
			UI::printUsers(Engine::getUsers());
			UI::systemMessage(Engine::chooseUser(user, UI::getUserChoice() - 1));
			break;
		case 6: // Create user
			user = UI::getNewUsername();
			UI::systemMessage(Engine::createUser(user));
		case 7: // Special options
			UI::printSpecialOptions();
			switch (UI::getUserChoice())
			{
			case 1: // Recreate the file with user data
				UI::systemMessage(Engine::recreateFile());
				break;
			case 2: // Delete the user
				UI::systemMessage(Engine::deleteUser(user));
				break;
			case 3: // Unfavorite a book
				UI::printBooks(Engine::getFavoriteBooks(user));
				UI::systemMessage(Engine::deleteFavoriteBook(user, UI::getUserChoice() - 1));
				break;
			case 4: // Test connection to the site
				UI::printTestConnectionRes(Engine::testConnection());
				break;
			case 5: // Nothing
				break;
			}
		case 8: // Exit
			return 0;
		}
	}
}
