#include "ui.h"

void UI::firstMessage()
{
	std::cout << "---LibraryViewer---\n";
}

void UI::printOptions()
{
	std::cout << "--------------------\n"
		<< "You can do:\n"
		<< "1. Open list of my books\n"
		<< "2. Look recently looked books\n"
		<< "3. Search\n"
		<< "4. Search randomly\n"
		<< "5. Choose user\n"
		<< "6. Create user\n"
		<< "7. Special options\n"
		<< "8. Exit\n"
		<< "1...8: ";
}

void UI::printSpecialOptions()
{
	std::cout << "--------------------\n"
		<< "You can do:\n"
		<< "1. Recreate the file with user data\n"
		<< "2. Delete the user\n"
		<< "3. Unfavorite a book\n"
		<< "4. Test connection to the site\n"
		<< "5. Return to previous menu\n"
		<< "1...5: ";
}

void UI::printAfterSearchOptions()
{
	std::cout << "--------------------\n"
		<< "You can:\n"
		<< "1. Look a book description\n"
		<< "2. Favorite a book\n"
		<< "3. Return to previous menu\n"
		<< "1...3: ";
}

short UI::getUserChoice()
{
	short choice{};
	std::cin >> choice;
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Invalid input. Defaulting 1\n";
		return 1;
	}
	return choice;
}

std::string UI::getNewUsername()
{
	std::string username;
	std::cout << "Enter the new user name: ";
	std::cin.ignore(1000, '\n');
	std::getline(std::cin, username);
	return username;
}

void UI::printChosenUser(std::string_view username)
{
	std::cout << "Now chosen ";
	if (username.empty())
	{
		std::cout << "guest";
	}
	else
	{
		std::cout << username;
	}
	std::cout << "\n";
}

void UI::printUsers(const std::vector<std::string>& users)
{
	std::cout << "Choose guest user or a user name:";
	std::cout << "\n1. Guest";
	for (int i{1}; i < users.size(); ++i)
	{
		std::cout << "\n" << i + 1 << ". " << users[i];
	}
	std::cout << "\n1..." << users.size() << ": ";
}

void UI::printBooks(const std::vector<Engine::Book>& books)
{
	if (books.empty())
	{
		std::cout << "No books to print\n";
		return;
	}
	for (int book{}; book < books.size(); ++book)
	{
		if (books.size() != 1)
		{
			std::cout << book + 1 << ". ";
		}
		std::cout << (books[book].title.empty() ? "No title" : books[book].title)
			<< "(" << (books[book].year == 0 ? "no publish year" : std::to_string(books[book].year)) << ", " 
			<< (books[book].author.empty() ? "no authors" : authorsVecToStr(books[book].author)) << ") in "
			<< (books[book].language.empty() ? "no languages" : languagesVecToStr(books[book].language))
			<< " - " << (books[book].link.empty() ? "failed to get link" : books[book].link) << "\n";
	}
}

void UI::printBookDescription(std::string_view description)
{
	if (description.empty())
	{
		std::cout << "No description for this book\n";
	}
	else
	{
		std::cout << description << "\n";
	}
}

void UI::printTestConnectionRes(const long status)
{
	std::cout << "Trying to connect to https://openlibrary.org/...\n";
	if (status == 200)
	{
		std::cout << "Connection successfull\n";
	}
	else
	{
		std::cout << "Failed to connect to https://openlibrary.org/. Error: " << status << "\n";
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
		std::cout << "Failed to find account with that name\n";
		return;
	case Engine::ResponseCode::FailedFileUpdate:
		std::cout << "Failed to update user data in file\n";
		return;
	case Engine::ResponseCode::EmptyUsername:
		std::cout << "Unable to create user without name, delete him and do the same with guest\n";
		return;
	case Engine::ResponseCode::CreatingIdenticalUser:
		std::cout << "User not created: a user with that name already exists\n";
		return;
	case Engine::ResponseCode::InvalidInput:
		std::cout << "Invalid input\n";
		return;
	case Engine::ResponseCode::GuestFavoriteOrRecentlyBook:
		std::cout << "Guests are not allowed to add, delete or have their own favorite or recently books\n";
		return;
	case Engine::ResponseCode::EmptyRandomBook:
		std::cout << "Failed to search random book\n";
		return;
	case Engine::ResponseCode::MaxCountOfFavoriteBooks:
		std::cout << "Failed to favorite book: max count of favorite book - 5";
		return;
	}
}

void UI::changeAuthorSearch(Engine::SearchParams& sParams)
{
	std::string newAuthor;
	sParams.authors.clear();
	std::cin.ignore(1000, '\n');
	while (true)
	{
		std::cout << "Enter the author or type minus (\"-\") to finish editing authors parameter: ";
		std::getline(std::cin, newAuthor);
		if (newAuthor == "-")
		{
			break;
		}
		sParams.authors.push_back(newAuthor);
	}
}

void UI::changeLanguageSearch(Engine::SearchParams& sParams)
{
	std::string newLang;
	sParams.langs.clear();
	std::cin.ignore(1000, '\n');
	while (true)
	{
		std::cout << "Enter the language name in English or type minus (\"-\") to finish editing language parameter: ";
		std::getline(std::cin, newLang);
		if (newLang == "-")
		{
			break;
		}
		std::transform(begin(newLang), end(newLang), begin(newLang), [](unsigned char symbol) { return std::tolower(symbol); }); // All symbols must be lowercase
		sParams.langs.push_back(newLang);
	}
}

void UI::changeTitleSearch(Engine::SearchParams& sParams)
{
	std::cout << "Enter the title or type minus (\"-\") to leave the field blank to search any title: ";
	std::cin.ignore(1000, '\n');
	std::getline(std::cin, sParams.title);
	if (sParams.title == "-")
	{
		sParams.title = "";
	}
}

void UI::changeYearSearch(Engine::SearchParams& sParams)
{
	sParams.year = 0;
	std::cout << "Enter the year or type zero (\"0\") to search any year: ";
	std::cin >> sParams.year;
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Invalid input. Defaulting 0\n";
		sParams.year = 0;
		return;
	}
	if (sParams.year < 0)
	{
		sParams.year = 0;
	}
}

void UI::changeSortSearch(Engine::SearchParams& sParams)
{
	int newSortMode{};
	std::cout << "Choose the sort mode:\n"
		<< "1. Relevant\n"
		<< "2. Count of editions\n"
		<< "3. Old\n"
		<< "4. New\n"
		<< "5. Rating\n"
		<< "1...5: ";
	std::cin >> newSortMode;
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Invalid input. Defaulting 1\n";
		sParams.sort = Engine::Sort::None;
		return;
	}
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
		std::cout << "Invalid choosed number. Defaulting sort mode \"relevant\"";
		sParams.sort = Engine::Sort::None;
	}
}

void UI::changeResListSizeSearch(Engine::SearchParams& sParams)
{
	std::cout << "Enter the size of search result list (1...100): ";
	std::cin >> sParams.resListSize;
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Invalid input. Defaulting 10\n";
		sParams.resListSize = 10;
		return;
	}
	if (sParams.resListSize < 1 || sParams.resListSize > 100)
	{
		std::cout << "Invalid input. Defaulting 10\n";
		sParams.resListSize = 10;
	}
}

void UI::changeSearchParams(const std::string& username, Engine::SearchParams& sParams)
{
	short choice{};
	std::cout << "Do you want to change the search parameters?\n1.Yes\n2.No\n1...2: ";
	std::cin >> choice;
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Invalid input. Defaulting 2\n";
		return;
	}
	if (choice != 1)
	{
		return;
	}
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
			std::cout << "Invalid input. Choose from 1 to 7\n";
		}
		UI::printSearchParams(sParams);
	}
}

void UI::printSearchParams(const Engine::SearchParams& sParams)
{
	std::cout << "Search params:"
		<< "\nAuthor - " << (sParams.authors.empty() ? "any" : authorsVecToStr(sParams.authors))
		<< "\nLanguage - " << (sParams.langs.empty() ? "any" : languagesVecToStr(sParams.langs))
		<< "\nTitle - " << (sParams.title.empty() ? "any" : sParams.title)
		<< "\nPublish year - " << (sParams.year == 0 ? "any" : std::to_string(sParams.year))
		<< "\nSort mode - " << sortModeToStr(sParams.sort)		
		<< "\nSearch results list size - " << sParams.resListSize << "\n";
}

void UI::printRandomSearchParams(const Engine::SearchParams& sParams)
{
	std::cout << "Search params:"
		<< "\nAuthor - " << (sParams.authors.empty() ? "any" : authorsVecToStr(sParams.authors))
		<< "\nLanguage - " << (sParams.langs.empty() ? "any" : languagesVecToStr(sParams.langs))
		<< "\nPublish year - " << (sParams.year == 0 ? "any" : std::to_string(sParams.year)) << "\n";
}

Engine::SearchParams UI::specifyingRandomSearchParams()
{
	Engine::SearchParams sParams;
	int choice{};
	std::string input;
	while (true)
	{
		std::cout << "Do you want to specify some search parameters?\n1.Yes\n2.No\n1...2: ";
		std::cin >> choice;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			std::cout << "Invalid input. Defaulting 2\n";
			return sParams;
		}
		if (choice == 1)
		{
			std::cout << "Which parameter do you want to change?\n1. Author\n2. Language\n3. Publish year\n1...3: ";
			std::cin >> choice;
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				std::cout << "Invalid input. Defaulting nothing\n";
				continue;
			}
			switch (choice)
			{
			case 1:
				std::cout << "Enter the author name: ";
				std::cin.ignore(1000, '\n');
				std::getline(std::cin, input);
				sParams.authors.push_back(input);
				break;
			case 2:
				std::cout << "Enter the name of language: ";
				std::cin.ignore(1000, '\n');
				std::getline(std::cin, input);
				sParams.langs.push_back(input);
				break;
			case 3:
				std::cout << "Enter the publish year: ";
				std::cin >> sParams.year;
				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(1000, '\n');
					std::cout << "Invalid input. Defaulting 0 (any year)\n";
					sParams.year = 0;
					break;
				}
				break;
			default:
				std::cout << "Invalid input\n";
			}
		}
		else
		{
			return sParams;
		}
		UI::printRandomSearchParams(sParams);
	}
}

void UI::lookBookDescriptionFromSearch(std::string_view username, const std::vector<Engine::Book>& books)
{
	short bookNumber{};
	std::cout << "Choose from book list above one of them to look the description: ";
	std::cin >> bookNumber;
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Invalid input. Defaulting nothing\n";
		return;
	}
	UI::printBookDescription(Engine::getBookDescription(books.at(bookNumber - 1).link));
	UI::systemMessage(Engine::addRecentlyBook(username, books.at(bookNumber - 1)));
}

void UI::favoriteBookFromSearch(std::string_view username, const std::vector<Engine::Book>& books)
{
	short bookNumber{};
	std::cout << "Choose from book list above one of them to favorite: ";
	std::cin >> bookNumber;
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Invalid input. Defaulting nothing\n";
		return;
	}
	UI::systemMessage(Engine::addFavoriteBook(username, books.at(bookNumber - 1)));
	UI::systemMessage(Engine::addRecentlyBook(username, books.at(bookNumber - 1)));
}

void UI::favoriteRandomBook(std::string_view username, const Engine::Book& book)
{
	short choice{};
	std::cout << "Do you want to favorite the book?\n1.Yes\n2.No\n1..2: ";
	std::cin >> choice;
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Invalid input. Defaulting 2\n";
		return;
	}
	if (choice != 1)
	{
		return;
	}
	UI::systemMessage(Engine::addFavoriteBook(username, book));
}

std::string UI::authorsVecToStr(const std::vector<std::string>& authorsV)
{
	std::string authorsS{ authorsV[0] };
	if (authorsV.size() == 1)
	{
		return authorsS;
	}
	for (int author{ 1 }; author < authorsV.size(); ++author)
	{
		authorsS += ", " + authorsV[author];
	}
	return authorsS;
}

std::string UI::languagesVecToStr(const std::vector<std::string>& langsV)
{
	std::string langsS{ langsV[0] };
	if (langsV.size() == 1)
	{
		return langsS;
	}
	for (int author{ 1 }; author < langsV.size(); ++author)
	{
		langsS += ", " + langsV[author];
	}
	return langsS;
}

std::string UI::sortModeToStr(const Engine::Sort& sort)
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
}
