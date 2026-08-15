#include <iostream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "engine.h"
#include "ui.h"

using json = nlohmann::json;

int main()
{
	UI::firstMessage();
	UI::beforeConnection();
	/*cpr::Response r = cpr::Get(cpr::Url{"https://openlibrary.org/search.json"},
		cpr::Parameters{ {"q", "the+lord+of+the+rings"} });
	json j = json::parse(r.text);*/
	//std::cout << j["docs"][0].dump(4);


	{
		long r = Engine::testConnection();
		if (r != 200)
		{
			UI::failedConnection(r);
			return 1;
		}
	}
	
	UI::successfulConnection();
	
	char choice{};
	std::string user;
	Engine::SearchParams sParams;
	std::vector<Book> books;
	while (true)
	{
		UI::printOptions();
		switch (UI::getUserChoice())
		{
		case 1:
			UI::printBooks(Engine::favoriteBooks(user));
			break;
		case 2:
			UI::printBooks(Engine::recentlyBooks(user));
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
			return 0;
		}
	}
}
