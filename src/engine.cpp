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

bool Engine::selectUser(std::string& userName)
{
	if (userName.length() > 0)
		return true;
	std::fstream file("userData.txt");
	if (file)
	{
		/*
		if (users.size() < 1) createUser() // enter the login
		else if (users.size() == 1) printBooksList(myBooks(user))
		else if (users.size() > 1)
			if (ensureUserSelected(&user)) printBooksList(myBooks(user))
			if (!user.empty()) myBooks(user)
			else cout << choose line number with your login; printUsers(); choosedUser; cin >> choosedUser; chooseUser(choosedUser, &user); // check user login
				*/
	}
}