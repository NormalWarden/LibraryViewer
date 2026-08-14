#include "engine.h"

long Engine::testConnection()
{
	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" });
	return r.status_code;
}

bool Engine::createUser(const std::string& username, json& userdata)
{
	// Checks
	if (username.empty())
	{
		std::cout << "\nUser not created: failed to create user without name";
		return false;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			std::cout << "\nUser not created: a user with that name already exists";
			return false;
		}
	}

	// Saving
	userdata["users"].push_back(json::parse(JSONTemplates::userTemplate));
	userdata["users"].back()["username"] = username;
	if (JSONToFile(userdata))
		return true;
	else
	{
		std::cout << "\nFailed to update user data in file";
		return false;
	}
}

bool Engine::chooseUser(std::string& username)
{
	json userdata{ fileToJSON() };

	if (userdata["users"].size() == 1)
	{
		username = userdata["users"][0]["username"];
		return USER_SELECTED;
	}
	else
	{
		int choice{};
		while (true)
		{
			std::cin >> choice;
			if (choice == 1)
			{
				username = "";
				return USER_SELECTED;
			}
			else if (choice == userdata["users"].size() + 1)
			{
				std::string userName;
				std::cout << "Enter the new user name: ";
				std::cin >> userName;
				createUser(username, userdata);
				username = userdata["users"].back()["username"];
				return USER_SELECTED;
			}
			else
			{
				try
				{
					username = userdata["users"].at(choice - 1)["username"];
					return USER_SELECTED;
				}
				catch (json::out_of_range)
				{
					std::cout << "\nInvalid number. Choose another one: ";
				}
			}
		}
	}
	return USER_SELECTED;
}

std::vector<std::string> Engine::getUsers(const std::string& filename)
{
	json data{ fileToJSON() };
	std::vector<std::string> users;
	
	for (int i{}; i < data["users"].size(); ++i)
		users.push_back(data["users"][i]["username"]);
	return users;
}

std::vector<Book> Engine::favoriteBooks(const std::string& username)
{
	json userdata{ fileToJSON() };
	std::vector<Book> books;

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			if (userdata["users"][i]["favoriteBooks"].empty())
			{
				std::cout << "User has no one favorite book\n";
				return books;
			}
			// TODO: finish it after completing func "addFavoriteBook"
			return books;
		}
	}

	return std::vector<Book>();
}

std::vector<Book> Engine::recentlyBooks(const std::string username)
{
	json userdata{ fileToJSON() };
	std::vector<Book> books;

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			if (userdata["users"][i]["recentlyBooks"].empty())
			{
				std::cout << "User has no one recently book\n";
				return books;
			}
			// TODO: finish it after completing func "addRecentlyBook"
			return books;
		}
	}

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
	if (sParams.year != 0)
		cprParams.Add({ "publish_year", std::to_string(sParams.year) });
	if (!sParams.author.empty())
		cprParams.Add({ "author", sParams.author });
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

Book Engine::randomSearch()
{
	SearchParams sParams;
	char choice{};
	std::string input;
	while (true)
	{
		std::cout << "Do you want to specify some search parameters?\n1. Yes\n2. No\n";
		std::cin >> choice;
		if (choice == 1)
		{
			std::cout << "\nWhich parameter do you want to change?\n1. Author\n2. Language\n3. Publish year\n";
			std::cin >> choice;
			switch (choice)
			{
			case 1:
				std::cout << "\nEnter the author name: ";
				std::cin >> input;
				sParams.author = input;
				break;
			case 2:
				std::cout << "\nEnter the name of language: ";
				std::cin >> input;
				sParams.langs.push_back(input);
				break;
			case 3:
				std::cout << "\nEnter the publish year: ";
				std::cin >> input;
				sParams.year = std::stoi(input);
				break;
			default:
				std::cout << "\nInvalid input. Changes reset";
			}
			std::cout << "\n";
		}
		else
			break;
	}
	cpr::Parameters cprParams;
	if (sParams.year != 0)
		cprParams.Add({ "publish_year", std::to_string(sParams.year) });
	if (!sParams.author.empty())
		cprParams.Add({ "author", sParams.author });
	if (!sParams.langs.empty())
		for (auto lang : sParams.langs)
			cprParams.Add({ "language", lang });

	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" }, cprParams);
	json j = json::parse(r.text);
	std::vector<Book> books;
	for (int el{ 0 }; el < 100; ++el)
	{
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
	std::random_device rd;
	std::mt19937 rng{ rd() };
	std::uniform_int_distribution<std::mt19937::result_type> uid{ 0,99 };
	return books[uid(rng)];
}

bool Engine::changeSearchParams(const std::string& username, SearchParams& sParams)
{
	char choice{};
	while (true)
	{
		std::cout << "What do you want to change:\n1. Author\n2. Language\n3. Title\n4. Year\n5. Sort mode\n6. Size of search result list\n7. Nothing\n1...7: ";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			changeAuthorSearch(sParams);
			break;
		case 2:
			changeLanguageSearch(sParams);
			break;
		case 3:
			changeTitleSearch(sParams);
			break;
		case 4:
			changeYearSearch(sParams);
			break;
		case 5:
			changeSortSearch(sParams);
			break;
		case 6:
			changeResListSizeSearch(sParams);
			break;
		case 7:
			if (saveSearchParams(username, sParams))
				return true;
			else
			{
				std::cout << "\nFailed to update user data in file";
				return false;
			}
		default:
			std::cout << "\nInvalid input. Choose from 1 to 7\n";
		}
	}
}

Engine::ResponseCode Engine::getSearchParams(const std::string& username, SearchParams& sParams)
{
	if (username.empty()) // guest
	{
		sParams.author = "";
		sParams.langs = { "eng" };
		sParams.title = "the lord of the rings";
		sParams.year = 1954;
		sParams.sort = Sort::None;
		sParams.resListSize = 10;
		return ResponseCode::Ok;
	}

	json userdata{ Engine::fileToJSON() };

	if (userdata.empty())
		return ResponseCode::EmptyJSON;
	for (int i{}; i < userdata["users"]; ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			sParams.author = userdata["users"][i]["searchParams"]["author"];
			sParams.langs = userdata["users"][i]["searchParams"]["language"];
			sParams.title = userdata["users"][i]["searchParams"]["title"];
			sParams.year = userdata["users"][i]["searchParams"]["year"];
			sParams.sort = userdata["users"][i]["searchParams"]["sort"];
			sParams.resListSize = userdata["users"][i]["searchParams"]["resListSize"];
			return ResponseCode::Ok;
		}
	}
	return ResponseCode::NoUser;
}

void Engine::printSearchParams(const SearchParams& sParams)
{
	std::cout << "Search params:"
		<< "\nAuthor: " << (sParams.author.empty() ? "any" : sParams.author)
		<< "\nLanguage: " << (sParams.langs.empty() ? "any" : 
			[](const std::vector<std::string>& langs)->std::string 
			{
				std::string langsStr; 
				for (auto lang : langs) 
					langsStr = langsStr + " " + lang;
				return langsStr; 
			}(sParams.langs))
		<< "\nTitle: " << (sParams.title.empty() ? "any" : sParams.title)
		<< "\nYear: " << (sParams.year == 0 ? "any" : std::to_string(sParams.year))
		<< "\nSort mode: " << 
		[](const Sort& sort)->std::string
		{
			switch (sort)
			{
			case Sort::None:
				return "relevant";
			case Sort::Editions:
				return "count of editions";
			case Sort::Old:
				return "old";
			case Sort::New:
				return "new";
			case Sort::Rating:
				return "rating";
			}
		}(sParams.sort)
		<< "\nSearch results list size: " << sParams.resListSize << "\n";
}

bool Engine::saveSearchParams(const std::string& username, const SearchParams& sParams)
{
	json userdata{ fileToJSON() };

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			userdata["users"][i]["searchParams"]["author"] = sParams.author;
			userdata["users"][i]["searchParams"]["language"] = sParams.langs;
			userdata["users"][i]["searchParams"]["title"] = sParams.title;
			userdata["users"][i]["searchParams"]["year"] = sParams.year;
			userdata["users"][i]["searchParams"]["sort"] = sParams.sort;
			userdata["users"][i]["searchParams"]["resListSize"] = sParams.resListSize;
			break;
		}
	}

	if (JSONToFile(userdata))
		return true;
	else
	{
		std::cout << "\nFailed to update user data in file";
		return false;
	}
}

void Engine::changeAuthorSearch(SearchParams& sParams)
{
	std::cout << "Enter the author or leave the field blank to search any author: ";
	std::cin >> sParams.author;
	std::cout << "\n";
}

void Engine::changeLanguageSearch(SearchParams& sParams)
{
	std::string newLang;
	sParams.langs.clear();
	while (true)
	{
		std::cout << "\nEnter the language name in English or leave the field blank to end editing language parameter: ";
		std::cin >> newLang;
		if (LangStorage::language.find(newLang) != LangStorage::language.end())
			sParams.langs.push_back(LangStorage::language[newLang]);
		std::cout << "\n";
	}
}

void Engine::changeTitleSearch(SearchParams& sParams)
{
	std::cout << "\nEnter the title or leave the field blank to search any title: ";
	std::cin >> sParams.title;
	std::cout << "\n";
}

void Engine::changeYearSearch(SearchParams& sParams)
{
	int newYear{};
	std::cout << "\nEnter the year or leave the field blank to search any year: ";
	std::cin >> newYear; // TODO: try-catch and checks for number from input
	sParams.year = newYear;
	std::cout << "\n";
}

void Engine::changeSortSearch(SearchParams& sParams)
{
	char newSortMode{};
	std::cout << "\nChoose the sort mode:\n1. Relevant\n2. Count of editions\n3. Old\n4. New\n5. Rating\n1...5: ";
	std::cin >> newSortMode;
	switch (newSortMode)
	{
	case 1:
		sParams.sort = Sort::None;
		break;
	case 2:
		sParams.sort = Sort::Editions;
		break;
	case 3:
		sParams.sort = Sort::Old;
		break;
	case 4:
		sParams.sort = Sort::New;
		break;
	case 5:
		sParams.sort = Sort::Rating;
		break;
	default:
		std::cout << "Invalid choosed number. Choosing sort mode \"relevant\"";
		sParams.sort = Sort::None;
	}
	std::cout << "\n";
}

void Engine::changeResListSizeSearch(SearchParams& sParams)
{
	int newResListSize{};
	std::cout << "\nEnter the size of search result list (1...100): ";
	std::cin >> newResListSize; // TODO: try-catch and checks for number from input
	sParams.resListSize = newResListSize;
	std::cout << "\n";
}

bool Engine::addFavoriteBook(const std::string& username, Book book)
{
	json userdata{ fileToJSON() };

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			userdata["users"][i]["favoriteBooks"].push_back({
				{"author", book.getAuthor()},
				{"language", book.getLanguage()},
				{"title", book.getTitle()},
				{"year", book.getYear()},
				{"link", book.getLink()},
				});
			break;
		}
	}

	if (JSONToFile(userdata))
		return true;
	else
	{
		std::cout << "\nFailed to update user data in file";
		return false;
	}
}

bool Engine::addRecentlyBook(const std::string& username, Book book)
{
	json userdata{ fileToJSON() };

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			userdata["users"][i]["recentlyBooks"].push_back({
				{"author", book.getAuthor()},
				{"language", book.getLanguage()},
				{"title", book.getTitle()},
				{"year", book.getYear()},
				{"link", book.getLink()},
				});
			break;
		}
	}

	if (JSONToFile(userdata))
		return true;
	else
	{
		std::cout << "\nFailed to update user data in file";
		return false;
	}
}

json Engine::fileToJSON(const std::string& filename)
{
	std::fstream file(filename);
	if (!file)
	{
		file.open(filename, std::ios::out); // create file
		file.close();
		file.open(filename);
	}

	json userdata;
	try
	{
		userdata = json::parse(file);
	}
	catch (...)
	{
		int choice{};
		std::cout << "\nUser data is broken. Would you correct it?\n1. Yes\n2. No\n1...2: ";
		std::cin >> choice;
		if (choice == 1)
		{
			file.close();
			file.open(filename, std::ios::out | std::ios::trunc);
			file << JSONTemplates::fullTemplate;
			userdata = json::parse(JSONTemplates::fullTemplate);
		}
		else
		{
			std::cout << "\nBad transfer info from file to json format";
			return json();
		}
	}
	file.close();
	return userdata;
}

bool Engine::JSONToFile(const json& userdata, const std::string& filename)
{
	std::fstream file(filename, std::ios::out | std::ios::trunc);
	if (!file.is_open())
		return false;
	file << userdata.dump(4);
	if (!file.good())
	{
		file.close();
		return false;
	}
	file.close();
	return true;
}
