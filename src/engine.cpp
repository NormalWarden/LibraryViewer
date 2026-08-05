#include "engine.h"

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

std::vector<Book> Engine::search(const SearchParams& sParams)
{
	cpr::Parameters cprParams;
	if (!sParams.title.empty())
	{
		cprParams.Add({ "q",
			[](const std::string& title)->std::string
			{
				std::string correctTitle = title;
				std::replace(begin(correctTitle), end(correctTitle), ' ', '+');
				return correctTitle;
			}(sParams.title) });
	}
	if (!sParams.author.empty())
		cprParams.Add({"author", sParams.author});
	if (!sParams.langs.empty())
		for (auto lang : sParams.langs)
			cprParams.Add({ "language", lang });
	if (sParams.sort != Sort::None)
	{
		switch (sParams.sort)
		{
		case Sort::Editions:
			cprParams.Add({ "sort", "editions" });
			break;
		case Sort::Old:
			cprParams.Add({ "sort", "old" });
			break;
		case Sort::New:
			cprParams.Add({ "sort", "new" });
			break;
		case Sort::Rating:
			cprParams.Add({ "sort", "rating" });
			break;
		}
	}

	cpr::Response r = cpr::Get(cpr::Url{"https://openlibrary.org/search.json"}, cprParams);
	json j = json::parse(r.text);
	std::vector<Book> books;
	for (int el{ 0 }; el < sParams.resListSize; ++el)
	{
		std::cout << "https://openlibrary.org" + j["docs"][el]["key"].get<std::string>() + "/" + j["docs"][el]["title"].get<std::string>();
		std::cout << [](const json& j, const int& iter)->std::string
			{
				std::string link{ "https://openlibrary.org" + j["docs"][iter]["key"].get<std::string>() + "/" + j["docs"][iter]["title"].get<std::string>() };
				std::replace(begin(link), end(link), ' ', '_');
				return link;
			}(j, el);
		books.push_back(Book(j["docs"][el]["author_name"],
			j["docs"][el]["language"],
			j["docs"][el]["title"],
			[](const json& j, const int& iter)->std::string
			{
				std::string link{ "https://openlibrary.org" + j["docs"][iter]["key"].get<std::string>() + "/" + j["docs"][iter]["title"].get<std::string>() };
				std::replace(begin(link), end(link), ' ', '_');
				return link;
			}(j, el),
			j["docs"][el]["first_publish_year"]));
	}
	return books;
}

void Engine::printBooksList(const json& userData)
{
}

void Engine::changeSearchParams(SearchParams& sParams)
{
}

Book Engine::randomSearch(std::string author, std::vector<std::string> language, size_t year)
{
	return Book();
}

bool Engine::chooseUser(std::string& userName)
{
	json userData{ fileToJSON() };

	std::cout << "Now chosen ";
	if (userName.empty())
		std::cout << "guest";
	else
		std::cout << userName;

	int choice{};
	std::cout << "\nDo you want to change account? \n1.Yes\n2.No\n";
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
	return USER_SELECTED;
}

void Engine::getSearchParams(const std::string& user, SearchParams& sParams)
{
	if (user.empty()) // guest
	{
		sParams.author = "";
		sParams.langs = { "eng" };
		sParams.title = "the lord of the rings";
		sParams.year = 1954;
		sParams.sort = Sort::None;
		sParams.resListSize = 10;
		return;
	}

	json userData{ Engine::fileToJSON() };

	if (userData.empty())
	{
		std::cout << "Can't get search parameters from file";
		return;
	}
	for (int el{}; el < userData["users"]; ++el)
	{
		if (userData["users"][el]["username"] == user)
		{
			sParams.author = userData["users"][el]["searchParams"]["author"];
			sParams.langs = userData["users"][el]["searchParams"]["language"];
			sParams.title = userData["users"][el]["searchParams"]["title"];
			sParams.year = userData["users"][el]["searchParams"]["year"];
			sParams.sort = userData["users"][el]["searchParams"]["sort"];
			sParams.resListSize = userData["users"][el]["searchParams"]["resListSize"];
			return;
		}
	}
	std::cout << "Can't find account with name: " << user;
	return;
}

json Engine::fileToJSON()
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
		{
			std::cout << "\nBad transfer info from file to json format";
			return json();
		}
	}
	file.close();
	return userData;
}
