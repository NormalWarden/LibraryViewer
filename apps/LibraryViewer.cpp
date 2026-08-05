#include <iostream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "engine.h"

using json = nlohmann::json;

int main()
{
	std::cout << "Trying to connect to https://openlibrary.org/...\n";
	/*cpr::Response r = cpr::Get(cpr::Url{"https://openlibrary.org/search.json"},
		cpr::Parameters{ {"q", "the+lord+of+the+rings"} });
	json j = json::parse(r.text);*/
	//std::cout << j["docs"][0].dump(4);



	if (Engine::testConnection() == CONNECTION_FAILURE)
	{
		std::cout << "Unable to connect to https://openlibrary.org/";
		return CONNECTION_FAILURE;
	}
	std::cout << "Connection successfull\n";
	
	size_t choice{};
	std::string user;
	Engine::SearchParams sParams;
	std::vector<Book> books;
	while (true)
	{
		std::cout << "1. Open list of my books\n"
			<< "2. Look recently looked books\n"
			<< "3. Search\n"
			<< "4. Search randomly\n"
			<< "5. Choose user\n"
			<< "6. Exit\n"
			<< "1...6: ";
		std::cin >> choice; // TODO: check for uint (may be entered number 0< or symbol)
		switch (choice)
		{
		case(1):
			if (Engine::chooseUser(user))
				//Engine::printBooksList(Engine::userBooks(user));
				return 0;
			else
			{
				std::cout << "File \"userData.txt\" can't be open from program directory. Exit...";
				return 1;
			}
			break;
		case(2):
			//if (Engine::selectUser(user)) Engine::printBooksList(Engine::recentlyBooks(user));
			break;
		case(3):
			Engine::getSearchParams(user, sParams);
			books = Engine::search(sParams);
			//while (choice != 2) // 2=No on next lines
				//break;
				/*std::cout << "Settings for search: author " << author << " language: " << language ...
				std::cout << "Would you like change them:\n1. Yes\n2. No"; cin >> choice; if (choice == 1) changeSearchParams(&author, &language, &title, &year, &sort, &length);
			printBooksList(search(author, language, title, year, sort, length))*/
			break;
			// TODO: create option to look book in more details
		case(4):
			/*while (choice != 2) // 2=No on next lines
				cout << "settings for search: author " << author << " language: " << language ...
				cout << "would u like change them:\n1. Yes\n2. No"; cin >> choice; if (choice == 1) changeSearchParams(&author, &language, &year);
			randomSearch(author, language, year);*/
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
