#include "engine.h"

enum class Engine::Sort
{
	None,
	Editions,
	Old,
	New,
	Rating
};

struct Engine::SearchParams
{
	std::string author;
	std::vector<std::string> langs;
	std::string title;
	size_t year{};
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

std::vector<Book> Engine::recentlyBooks(std::string user)
{
	return std::vector<Book>();
}

std::vector<Book> Engine::search(const SearchParams& params)
{
	cpr::Parameters searchParams;
	if (!params.author.empty())
		searchParams.Add({"author", params.author});
	if (!params.langs.empty())
		for (auto lang : params.langs)
			searchParams.Add({ "language", lang });
	if (!params.title.empty())
		searchParams.Add({ "q", params.title });
	if (params.sort != Sort::None)
	{
		switch (params.sort)
		{
		case Sort::Editions:
			searchParams.Add({ "sort", "editions" });
			break;
		case Sort::Old:
			searchParams.Add({ "sort", "old" });
			break;
		case Sort::New:
			searchParams.Add({ "sort", "new" });
			break;
		case Sort::Rating:
			searchParams.Add({ "sort", "rating" });
			break;
		}
	}

	cpr::Response r = cpr::Get(cpr::Url{"https://openlibrary.org/search.json"}, searchParams);
	json j = json::parse(r.text);
	std::vector<Book> books;
	for (int el{ 0 }; el < params.resListSize; ++el)
		books.push_back(Book(j["docs"]["author_name"], 
			j["docs"]["language"], 
			j["docs"]["title"], 
			[](const json& j)->std::string 
			{ 
				std::string link{ "https://openlibrary.org/" + j["docs"]["key"] + j["docs"]["title"] };
				link.replace(begin(link), end(link), " ", "_");
				return link;  
			}(j),
			j["docs"]["first_publish_year"]));
	return books;
}

void Engine::printBooksList(const json& userData)
{
}

void Engine::changeSearchParams(SearchParams& params)
{
}

Book Engine::randomSearch(std::string author, std::vector<std::string> language, size_t year)
{
	return Book();
}

bool Engine::chooseUser(std::string& userName)
{
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
		}
		else
			return USER_NOT_SELECTED;
	}

	std::cout << "Now chosen ";
	if (userName.length() == 0)
		std::cout << "guest";
	else
		std::cout << userName;

	int choice{};
	std::cout << "\nDo you want to change account ? \n1.Yes\n2.No\n";
	std::cin >> choice;
	if (choice == 1)
	{
		if (userData["users"].size() < 1)
		{
			createUser(userData);
			userName = userData["users"][0]["username"];
			return USER_SELECTED;
		}
		else if (userData["users"].size() == 1)
		{
			userName = userData["users"][0]["username"];
			return USER_SELECTED;
		}
		else
		{
			std::cout << "Choose guest user, your user name or create new one:";
			std::cout << "\n1. Guest";
			for (int i{ 0 }; i < userData["users"].size(); ++i)
				std::cout << "\n" << i + 2 << ". " << userData["users"][i]["username"];
			std::cout << "\n" << userData["users"].size() + 2 << ". Create new one\n1..." << userData["users"].size() + 2 << ": ";
			int choice{};
			while (true)
			{
				std::cin >> choice;
				if (choice == 1)
				{
					userName = "";
					return USER_SELECTED;
				}
				else if (choice == userData["users"].size() + 1)
				{
					createUser(userData);
					userName = userData["users"].back()["username"];
					return USER_SELECTED;
				}
				else
				{
					try
					{
						userName = userData["users"].at(choice - 1)["username"];
						return USER_SELECTED;
					}
					catch (json::out_of_range)
					{
						std::cout << "\nInvalid number. Choose another one: ";
					}
				}
			}
		}
	}
	file.close();
	return USER_SELECTED;
}

Engine::SearchParams& Engine::getSearchParams(const std::string&)
{
	SearchParams userParams;
	return userParams;
}
