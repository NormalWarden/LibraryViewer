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
		case(1):
			UI::printBooks(Engine::favoriteBooks(user));
			break;
		case(2):
			UI::printBooks(Engine::recentlyBooks(user));
			break;
		case(3):
			UI::systemMessage(Engine::getSearchParams(user, sParams));
			while (true)
			{
				Engine::printSearchParams(sParams);
				std::cout << "\nDo you want to change the search parameters?\n1. Yes\n2. No\n";
				std::cin >> choice;
				std::cout << "\n";
				if (choice == 1)
				{
					Engine::changeSearchParams(user, sParams);
				}
			}
			UI::printBooks(Engine::search(sParams));
			break;
			// TODO: create option to look book in more details
		case(4):
			Engine::randomSearch();
			break;
			// TODO: create option to look book in more details
		case(5):
			Engine::chooseUser(user);
			break;
		case(6):
			std::cout << "Do you really want to exit ?\n1.Yes\n2.No\n1...2: "; 
			std::cin >> choice; 
			if (choice == 1) return 0;
			break;
		}
	}
	return 0;
}
