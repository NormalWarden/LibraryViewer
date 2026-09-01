#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "engine.h"
#include "ui.h"

using json = nlohmann::json;

int main()
{
	std::string user;
	Engine::SearchParams sParams;

	UI::firstMessage();
	UI::printTestConnectionRes(Engine::testConnection());
	std::vector<Book> books{};
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
			books = Engine::search(sParams);
			UI::printBooks(books);
			UI::printAfterSearchOptions();
			for (short choice{ UI::getUserChoice() }; choice != 3; choice = UI::getUserChoice())
			{
				switch (choice)
				{
				case 1:
					UI::lookBookDescriptionFromSearch(user, books);
					break;
				case 2:
					UI::favoriteBookFromSearch(user, books);
					break;
				}
			}
			break;
		case 4: // Search randomly
			books.clear();
			books.push_back(Engine::randomSearch(UI::specifyingRandomSearchParams()));
			UI::printBooks(books);
			UI::printBookDescription(Engine::getBookDescription(books[0].getLink()));
			UI::systemMessage(Engine::addRecentlyBook(user, books[0]));
			UI::favoriteRandomBook(user, books[0]);
			break;
		case 5: // Choose user
			UI::printChosenUser(user);
			UI::printUsers(Engine::getUsers());
			UI::systemMessage(Engine::chooseUser(user, UI::getUserChoice() - 1));
			break;
		case 6: // Create user
			user = UI::getNewUsername();
			UI::systemMessage(Engine::createUser(user));
			break;
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
			break;
		case 8: // Exit
			return 0;
		}
	}
}
