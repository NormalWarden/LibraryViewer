#include "engine.h"

enum class Engine::Sort
{
	None,
	Editions,
	Old,
	New,
	Rating
};

enum class Engine::YearSearch
{
	Before,
	Current,
	After
};

struct Engine::SearchParams
{
	std::string author = "Any";
	std::vector<std::string> langs;
	std::string name = "Any";
	size_t year = NULL;
	YearSearch yearSearch = YearSearch::After;
	Sort sort = Sort::None;
	size_t resListSize = 10;
};

bool Engine::testConnection()
{
	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" });
	return r.status_code == 200 ? CONNECTION_SUCCESS : CONNECTION_FAILURE;
}

void Engine::createUser(json& userData)
{
	std::string userName;
	std::cout << "Enter the name new user: ";
	std::cin >> userName;
	userData["users"].push_back(json::parse(JSONTemplates::userTemplate));
	userData["users"].back()["username"] = userName;
	std::fstream file(USERDATA_FILENAME, std::ios::out | std::ios::trunc);
	file << userData.dump(4);
	file.close();
}

std::vector<Book> Engine::userBooks(std::string user)
{
	return std::vector<Book>();
}

void Engine::printUsers()
{
}

void Engine::chooseUser(std::string choosedUser, std::string& user)
{
}

std::vector<Book> Engine::recentlyBooks(std::string user)
{
	return std::vector<Book>();
}

void Engine::printBooksList(std::vector<Book> books)
{
}

void Engine::changeSearchParams(SearchParams& params)
{
}

Book Engine::randomSearch(std::string author, std::vector<std::string> language, size_t year)
{
	return Book();
}

bool Engine::selectUser(std::string& userName)
{
	if (userName.length() > 0)
		return USER_SELECTED;
	std::fstream file(USERDATA_FILENAME);
	if (!file)
	{
		file.close();
		file.open(USERDATA_FILENAME, std::ios::out); // create file
		file.close();
		file.open(USERDATA_FILENAME);
	}
	
	json userData;
	try
	{
		userData = json::parse(file);
	}
	catch (...)
	{
		int choice{};
		std::cout << "\nUser data is broken. Would you correct it?\n1. Yes\n2. No\n1...2: ";
		std::cin >> choice;
		if (choice == 1)
		{
			file.close();
			file.open(USERDATA_FILENAME, std::ios::out | std::ios::trunc);
			file << JSONTemplates::fullTemplate;
			userData = json::parse(JSONTemplates::fullTemplate);
			file.close();
		}
		else
			return USER_NOT_SELECTED;
	}
	if (userData["users"].size() < 1) createUser(userData);
	//else if (users.size() == 1) printBooksList(myBooks(user))
	//else if (users.size() > 1)
	//	if (ensureUserSelected(&user)) printBooksList(myBooks(user))
	//	if (!user.empty()) myBooks(user)
	//	else cout << choose line number with your login; printUsers(); choosedUser; cin >> choosedUser; chooseUser(choosedUser, &user); // check user login
	//		
	file.close();
	return USER_SELECTED;
}