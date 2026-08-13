#include "engine.h"

long Engine::testConnection()
{
	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" });
	return r.status_code;
}

bool Engine::createUser(json& userData)
{
	std::string userName;
	std::cout << "Enter the new user name: ";
	std::cin >> userName;

	// Checks
	if (userName.empty())
	{
		std::cout << "\nUser not created: failed to create user without name";
		return false;
	}
	for (int i{}; i < userData["users"].size(); ++i)
	{
		if (userData["users"][i]["username"] == userName)
		{
			std::cout << "\nUser not created: a user with that name already exists";
			return false;
		}
	}

	// Saving
	userData["users"].push_back(json::parse(JSONTemplates::userTemplate));
	userData["users"].back()["username"] = userName;
	if (JSONToFile(userData))
		return true;
	else
	{
		std::cout << "\nFailed to update user data in file";
		return false;
	}
}

std::vector<Book> Engine::favoriteBooks(const std::string& user)
{
	json userData{ fileToJSON() };
	std::vector<Book> books;

	for (int i{}; i < userData["users"].size(); ++i)
	{
		if (userData["users"][i]["username"] == user)
		{
			if (userData["users"][i]["favoriteBooks"].empty())
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

std::vector<Book> Engine::recentlyBooks(std::string user)
{
	json userData{ fileToJSON() };
	std::vector<Book> books;

	for (int i{}; i < userData["users"].size(); ++i)
	{
		if (userData["users"][i]["username"] == user)
		{
			if (userData["users"][i]["recentlyBooks"].empty())
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

void Engine::printBooksList(const std::vector<Book>& books)
{
	for (int book{}; book < books.size(); ++book)
	{
		std::cout << book << ". " << books[book].getTitle() 
			<< "(" << books[book].getYear() << ", "
			<< [](const std::vector<std::string>& authorsV)->std::string
			{
				std::string authorsS;
				for (auto author : authorsV)
					authorsS = authorsS + ", " + author;
				return authorsS;
			}(books[book].getAuthor())
			<< ") in "
			<< [](const std::vector<std::string>& langsV)->std::string
			{
				std::string langsS;
				for (auto lang : langsV)
					langsS = langsS + "," + lang;
				return langsS;
			}(books[book].getLanguage())
			<< " - " << books[book].getLink() << "\n";
	}
}

bool Engine::changeSearchParams(const std::string& user, SearchParams& sParams)
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
			if (saveSearchParams(user, sParams))
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
		std::cout << "Can't get search parameters from file\n";
		return;
	}
	for (int i{}; i < userData["users"]; ++i)
	{
		if (userData["users"][i]["username"] == user)
		{
			sParams.author = userData["users"][i]["searchParams"]["author"];
			sParams.langs = userData["users"][i]["searchParams"]["language"];
			sParams.title = userData["users"][i]["searchParams"]["title"];
			sParams.year = userData["users"][i]["searchParams"]["year"];
			sParams.sort = userData["users"][i]["searchParams"]["sort"];
			sParams.resListSize = userData["users"][i]["searchParams"]["resListSize"];
			return;
		}
	}
	std::cout << "Can't find account with name: " << user << "\n";
}

json Engine::fileToJSON()
{
	std::fstream file(USERDATA_FILENAME);
	if (!file)
	{
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

bool Engine::saveSearchParams(const std::string& user, const SearchParams& sParams)
{
	json userData{ fileToJSON() };

	for (int i{}; i < userData["users"].size(); ++i)
	{
		if (userData["users"][i]["username"] == user)
		{
			userData["users"][i]["searchParams"]["author"] = sParams.author;
			userData["users"][i]["searchParams"]["language"] = sParams.langs;
			userData["users"][i]["searchParams"]["title"] = sParams.title;
			userData["users"][i]["searchParams"]["year"] = sParams.year;
			userData["users"][i]["searchParams"]["sort"] = sParams.sort;
			userData["users"][i]["searchParams"]["resListSize"] = sParams.resListSize;
			break;
		}
	}

	if (JSONToFile(userData))
		return true;
	else
	{
		std::cout << "\nFailed to update user data in file";
		return false;
	}
}

bool Engine::addFavoriteBook(const std::string& user, Book book)
{
	json userData{ fileToJSON() };

	for (int i{}; i < userData["users"].size(); ++i)
	{
		if (userData["users"][i]["username"] == user)
		{
			userData["users"][i]["favoriteBooks"].push_back({
				{"author", book.getAuthor()},
				{"language", book.getLanguage()},
				{"title", book.getTitle()},
				{"year", book.getYear()},
				{"link", book.getLink()},
				});
			break;
		}
	}

	if (JSONToFile(userData))
		return true;
	else
	{
		std::cout << "\nFailed to update user data in file";
		return false;
	}
}

bool Engine::addRecentlyBook(const std::string& user, Book book)
{
	json userData{ fileToJSON() };

	for (int i{}; i < userData["users"].size(); ++i)
	{
		if (userData["users"][i]["username"] == user)
		{
			userData["users"][i]["recentlyBooks"].push_back({
				{"author", book.getAuthor()},
				{"language", book.getLanguage()},
				{"title", book.getTitle()},
				{"year", book.getYear()},
				{"link", book.getLink()},
				});
			break;
		}
	}

	if (JSONToFile(userData))
		return true;
	else
	{
		std::cout << "\nFailed to update user data in file";
		return false;
	}
}

bool Engine::JSONToFile(const json& userData)
{
	std::fstream file(USERDATA_FILENAME, std::ios::out | std::ios::trunc);
	if (!file.is_open())
		return false;
	file << userData.dump(4);
	if (!file.good())
	{
		file.close();
		return false;
	}
	file.close();
	return true;
}
