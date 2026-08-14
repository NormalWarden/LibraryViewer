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

void UI::failedConnection(long status)
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
		<< "6. Exit\n"
		<< "1...6: ";
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
	std::cout << "\n" << users.size() + 2 << ". Create new one\n";
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

void UI::systemMessage(Engine::ResponseCode response)
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
	}
}
