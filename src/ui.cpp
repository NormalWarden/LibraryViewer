#include "ui.h"

void UI::firstMessage()
{
	std::cout << "---LibraryViewer---\n";
}

void UI::beforeConnection()
{
	std::cout << "Trying to connect to https://openlibrary.org/...\n";
}

void UI::successfulConnection()
{
	std::cout << "Connection successfull\n";
}

void UI::failedConnection(const long status)
{
	std::cout << "Failed to connect to https://openlibrary.org/. Error: " << status << "\n";
}

void UI::printOptions()
{
	std::cout << "1. Open list of my books\n"
		<< "2. Look recently looked books\n"
		<< "3. Search\n"
		<< "4. Search randomly\n"
		<< "5. Choose user\n"
		<< "6. Create user\n"
		<< "7. Exit\n"
		<< "1...7: ";
}

short UI::getUserChoice()
{
	short choice{};
	std::cin >> choice;
	return choice;
}

void UI::printChosenUser(std::string_view username)
{
	std::cout << "Now chosen ";
	if (username.empty())
		std::cout << "guest";
	else
		std::cout << username;
}

void UI::printUsers(const std::vector<std::string_view>& users)
{
	std::cout << "Choose guest user, a user name or create new one:";
	std::cout << "\n1. Guest";
	for (int i{}; i < users.size(); ++i)
		std::cout << "\n" << i + 2 << ". " << users[i];
	std::cout << "\n";
}

void UI::printBooks(const std::vector<Book>& books)
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

void UI::systemMessage(const Engine::ResponseCode response)
{
	switch (response)
	{
	case Engine::ResponseCode::Ok:
		return;
	case Engine::ResponseCode::EmptyJSON:
		std::cout << "Failed to get infotmation from file with user data\n";
		return;
	case Engine::ResponseCode::NoUser:
		std::cout << "Can't find account with that name\n";
		return;
	case Engine::ResponseCode::FailedFileUpdate:
		std::cout << "Failed to update user data in file\n";
		return;
	case Engine::ResponseCode::EmptyUsername:
		std::cout << "User not created: failed to create user without name\n";
		return;
	case Engine::ResponseCode::CreatingIdenticalUser:
		std::cout << "\nUser not created: a user with that name already exists";
		return;
	}
}

void UI::printSearchParams(const Engine::SearchParams& sParams)
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
		[](const Engine::Sort& sort)->std::string
		{
			switch (sort)
			{
			case Engine::Sort::None:
				return "relevant";
			case Engine::Sort::Editions:
				return "count of editions";
			case Engine::Sort::Old:
				return "old";
			case Engine::Sort::New:
				return "new";
			case Engine::Sort::Rating:
				return "rating";
			default:
				return "relevant";
			}
		}(sParams.sort)
			<< "\nSearch results list size: " << sParams.resListSize << "\n";
}

void UI::changeAuthorSearch(Engine::SearchParams& sParams)
{
	std::cout << "\nEnter the author or leave the field blank to search any author: ";
	std::cin >> sParams.author;
	std::cout << "\n";
}

void UI::changeLanguageSearch(Engine::SearchParams& sParams)
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

void UI::changeTitleSearch(Engine::SearchParams& sParams)
{
	std::cout << "\nEnter the title or leave the field blank to search any title: ";
	std::cin >> sParams.title;
	std::cout << "\n";
}

void UI::changeYearSearch(Engine::SearchParams& sParams)
{
	char newYear{};
	std::cout << "\nEnter the year or leave the field blank to search any year: ";
	std::cin >> newYear;
	sParams.year = newYear;
	std::cout << "\n";
}

void UI::changeSortSearch(Engine::SearchParams& sParams)
{
	char newSortMode{};
	std::cout << "\nChoose the sort mode:\n1. Relevant\n2. Count of editions\n3. Old\n4. New\n5. Rating\n1...5: ";
	std::cin >> newSortMode;
	switch (newSortMode)
	{
	case 1:
		sParams.sort = Engine::Sort::None;
		break;
	case 2:
		sParams.sort = Engine::Sort::Editions;
		break;
	case 3:
		sParams.sort = Engine::Sort::Old;
		break;
	case 4:
		sParams.sort = Engine::Sort::New;
		break;
	case 5:
		sParams.sort = Engine::Sort::Rating;
		break;
	default:
		std::cout << "Invalid choosed number. Choosing sort mode \"relevant\"";
		sParams.sort = Engine::Sort::None;
	}
	std::cout << "\n";
}

void UI::changeResListSizeSearch(Engine::SearchParams& sParams)
{
	int newResListSize{};
	std::cout << "\nEnter the size of search result list (1...100): ";
	std::cin >> newResListSize; // TODO: try-catch and checks for number from input
	sParams.resListSize = newResListSize;
	std::cout << "\n";
}

void UI::changeSearchParams(const std::string& username, Engine::SearchParams& sParams)
{
	char choice{};
	std::cout << "Do you want to change the search parameters (Y/N)?\n";
	std::cin >> choice;
	std::cout << "\n";
	
	if (choice != 'Y')
		return;
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
			return;
		default:
			std::cout << "\nInvalid input. Choose from 1 to 7\n";
		}
	}
}

Engine::SearchParams UI::specifyingRandomSearchParams()
{
	Engine::SearchParams sParams;
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
				std::cout << "\nInvalid input";
			}
			std::cout << "\n";
		}
		else
			return sParams;
	}
}

std::string UI::getNewUsername()
{
	std::string username;
	std::cout << "Enter the new user name: ";
	std::cin >> username;
	std::cout << "\n";
	return username;
}
