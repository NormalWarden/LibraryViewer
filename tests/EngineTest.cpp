#pragma once

#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <string>
#include "engine.h"
#include "jsonTemplates.h"

using json = nlohmann::json;


std::string fileToString()
{
	std::string filedata, fileline;
	std::fstream file{ Engine::FILENAME.data() };
	std::getline(file, fileline);
	filedata = fileline;
	while (std::getline(file, fileline))
	{
		filedata += "\n" + fileline;
	}
	file.close();
	return filedata;
}


TEST(EngineTest, TestConnection)
{
	ASSERT_EQ(Engine::testConnection(), 200);
}


TEST(EngineTest, RecreateFile_Ok)
{
	Engine::FILENAME = "test.txt";

	ASSERT_EQ(Engine::recreateFile(), Engine::ResponseCode::Ok);
	ASSERT_EQ(fileToString(), JSONTemplates::startTemplate);
}

TEST(EngineTest, RecreateFile_FailedFileOpen)
{
	Engine::FILENAME = "test/test.txt";
	// fstream with ios::out modifier can't create a folder but only a file
	ASSERT_EQ(Engine::recreateFile(), Engine::ResponseCode::FailedFileOpen);
}


TEST(EngineTest, FileToJSON_Ok)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	json filedata, templateData;
	templateData = json::parse(JSONTemplates::startTemplate);

	ASSERT_EQ(Engine::fileToJSON(filedata), Engine::ResponseCode::Ok);
	ASSERT_EQ(filedata, templateData);
}

TEST(EngineTest, FileToJSON_FailedFileOpen)
{
	Engine::FILENAME = "test/test.txt";
	json userdata;
	// fstream with ios::out modifier can't create a folder but only a file
	ASSERT_EQ(Engine::fileToJSON(userdata), Engine::ResponseCode::FailedFileOpen);
}


TEST(EngineTest, JSONToFile_Ok)
{
	Engine::FILENAME = "test.txt";
	json userdata{ JSONTemplates::startTemplate };

	ASSERT_EQ(Engine::JSONToFile(userdata), Engine::ResponseCode::Ok);
	ASSERT_EQ(fileToString(), userdata.dump(4));
}

TEST(EngineTest, JSONToFile_FailedFileOpen)
{
	Engine::FILENAME = "test/test.txt";
	json userdata{ JSONTemplates::startTemplate };
	// fstream with ios::out modifier can't create a folder but only a file
	ASSERT_EQ(Engine::JSONToFile(userdata), Engine::ResponseCode::FailedFileOpen);
}


TEST(EngineTest, GetUsers_Success)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::getUsers().size(), 1); // only guest
}

TEST(EngineTest, GetUsers_FailedOpenFile)
{
	Engine::FILENAME = "test/test.txt";
	ASSERT_EQ(Engine::getUsers().size(), 0);
}


TEST(EngineTest, CreateUser_Ok)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	std::string username = "new";

	ASSERT_EQ(Engine::createUser(username), Engine::ResponseCode::Ok);
}

TEST(EngineTest, CreateUser_CreatingIdenticalUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);

	ASSERT_EQ(Engine::createUser(username), Engine::ResponseCode::CreatingIdenticalUser);
}

TEST(EngineTest, CreateUser_EmptyUsername)
{
	Engine::FILENAME = "test.txt";
	std::string username = "";

	ASSERT_EQ(Engine::createUser(username), Engine::ResponseCode::EmptyUsername);
}


TEST(EngineTest, ChooseUser_OkGuest)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	std::string username = "";

	ASSERT_EQ(Engine::chooseUser(username, 0), Engine::ResponseCode::Ok); // Chosen "guest"
	ASSERT_EQ(username, "");
}

TEST(EngineTest, ChooseUser_OkUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);

	ASSERT_EQ(Engine::chooseUser(username, 1), Engine::ResponseCode::Ok); // Chosen "new"
	ASSERT_EQ(username, "new");
}

TEST(EngineTest, ChooseUser_InvalidInputGuest)
{
	Engine::FILENAME = "test.txt";
	std::string username = "";

	ASSERT_EQ(Engine::chooseUser(username, -1), Engine::ResponseCode::InvalidInput);
}

TEST(EngineTest, ChooseUser_InvalidInputUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);

	ASSERT_EQ(Engine::chooseUser(username, -1), Engine::ResponseCode::InvalidInput);
}


TEST(EngineTest, DeleteUser_Ok)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);

	ASSERT_EQ(Engine::deleteUser(username), Engine::ResponseCode::Ok);
}

TEST(EngineTest, DeleteUser_NoUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	std::string username = "new";

	ASSERT_EQ(Engine::deleteUser(username), Engine::ResponseCode::NoUser);
}

TEST(EngineTest, DeleteUser_EmptyUsername)
{
	Engine::FILENAME = "test.txt";
	std::string username = "";

	ASSERT_EQ(Engine::deleteUser(username), Engine::ResponseCode::EmptyUsername);
}


TEST(EngineTest, GetFavoriteBooks_OkUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);

	ASSERT_EQ(Engine::getFavoriteBooks("user").size(), 1);
}

TEST(EngineTest, GetFavoriteBooks_GuestFavoriteBook)
{
	Engine::FILENAME = "test.txt";

	ASSERT_EQ(Engine::getFavoriteBooks("").size(), 0); // guest has 0 favorite books
}

TEST(EngineTest, GetFavoriteBooks_NoUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::getFavoriteBooks("user").size(), 0);
}


TEST(EngineTest, AddFavoriteBook_Ok)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);
	Engine::Book favoriteBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };
	std::vector<Engine::Book> booksVTemplate{ favoriteBook };

	ASSERT_EQ(Engine::addFavoriteBook(username, favoriteBook), Engine::ResponseCode::Ok);
	ASSERT_EQ(Engine::getFavoriteBooks(username).size(), 1);
	EXPECT_EQ(Engine::getFavoriteBooks(username)[0].author, favoriteBook.author);
	EXPECT_EQ(Engine::getFavoriteBooks(username)[0].language, favoriteBook.language);
	EXPECT_EQ(Engine::getFavoriteBooks(username)[0].title, favoriteBook.title);
	EXPECT_EQ(Engine::getFavoriteBooks(username)[0].link, favoriteBook.link);
	EXPECT_EQ(Engine::getFavoriteBooks(username)[0].year, favoriteBook.year);
}

TEST(EngineTest, AddFavoriteBook_GuestFavoriteOrRecentlyBook)
{
	Engine::FILENAME = "test.txt";
	Engine::Book favoriteBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };

	ASSERT_EQ(Engine::addFavoriteBook("", favoriteBook), Engine::ResponseCode::GuestFavoriteOrRecentlyBook);
	ASSERT_EQ(Engine::getFavoriteBooks("").size(), 0);
}

TEST(EngineTest, AddFavoriteBook_NoUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	Engine::Book favoriteBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };

	ASSERT_EQ(Engine::addFavoriteBook("user", favoriteBook), Engine::ResponseCode::NoUser);
	ASSERT_EQ(Engine::getFavoriteBooks("user").size(), 0);
}


TEST(EngineTest, DeleteFavoriteBook_Ok)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);

	ASSERT_EQ(Engine::deleteFavoriteBook("user", 0), Engine::ResponseCode::Ok);
	ASSERT_EQ(Engine::getFavoriteBooks("user").size(), 0);
}

TEST(EngineTest, DeleteFavoriteBook_GuestFavoriteBook)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::deleteFavoriteBook("", 0), Engine::ResponseCode::GuestFavoriteOrRecentlyBook);
}

TEST(EngineTest, DeleteFavoriteBook_NoUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::deleteFavoriteBook("user", 0), Engine::ResponseCode::NoUser);
	ASSERT_EQ(Engine::getFavoriteBooks("user").size(), 0);
}


TEST(EngineTest, GetRecentlyBooks_OkUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);

	ASSERT_EQ(Engine::getRecentBooks("user").size(), 1);
}

TEST(EngineTest, GetRecentlyBooks_GuestFavoriteBook)
{
	Engine::FILENAME = "test.txt";

	ASSERT_EQ(Engine::getRecentBooks("").size(), 0); // guest has 0 recently books
}

TEST(EngineTest, GetRecentlyBooks_NoUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::getRecentBooks("user").size(), 0);
}


TEST(EngineTest, AddRecentlyBook_Ok)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);
	Engine::Book recentlyBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };

	ASSERT_EQ(Engine::addRecentBook(username, recentlyBook), Engine::ResponseCode::Ok);
	ASSERT_EQ(Engine::getRecentBooks(username).size(), 1);
	EXPECT_EQ(Engine::getRecentBooks(username)[0].author, recentlyBook.author);
	EXPECT_EQ(Engine::getRecentBooks(username)[0].language, recentlyBook.language);
	EXPECT_EQ(Engine::getRecentBooks(username)[0].title, recentlyBook.title);
	EXPECT_EQ(Engine::getRecentBooks(username)[0].link, recentlyBook.link);
	EXPECT_EQ(Engine::getRecentBooks(username)[0].year, recentlyBook.year);
}

TEST(EngineTest, AddRecentlyBook_GuestFavoriteOrRecentlyBook)
{
	Engine::FILENAME = "test.txt";
	Engine::Book recentlyBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };

	ASSERT_EQ(Engine::addRecentBook("", recentlyBook), Engine::ResponseCode::GuestFavoriteOrRecentlyBook);
	ASSERT_EQ(Engine::getRecentBooks("").size(), 0);
}

TEST(EngineTest, AddRecentlyBook_NoUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	Engine::Book recentlyBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };

	ASSERT_EQ(Engine::addRecentBook("user", recentlyBook), Engine::ResponseCode::NoUser);
	ASSERT_EQ(Engine::getRecentBooks("user").size(), 0);
}


TEST(EngineTest, DeleteRecentlyBook_Ok)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);

	ASSERT_EQ(Engine::deleteRecentBook("user", 0), Engine::ResponseCode::Ok);
	ASSERT_EQ(Engine::getRecentBooks("user").size(), 0);
}

TEST(EngineTest, DeleteRecentlyBook_GuestFavoriteBook)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::deleteRecentBook("", 0), Engine::ResponseCode::GuestFavoriteOrRecentlyBook);
}

TEST(EngineTest, DeleteRecentlyBook_NoUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::deleteRecentBook("user", 0), Engine::ResponseCode::NoUser);
	ASSERT_EQ(Engine::getRecentBooks("user").size(), 0);
}


TEST(EngineTest, GetBookDescription_Ok)
{
	ASSERT_EQ(Engine::getBookDescription("https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings").empty(), false);
}

TEST(EngineTest, GetBookDescription_EmptyLink)
{
	ASSERT_EQ(Engine::getBookDescription("").empty(), true);
}

TEST(EngineTest, GetBookDescription_BadLink)
{
	ASSERT_EQ(Engine::getBookDescription("https://openlibrary.org/works/BAD/The_Lord_of_the_Rings").empty(), true);
}


TEST(EngineTest, Search)
{
	Engine::SearchParams sParams{ {}, {}, "The lord of the rings", 0, Engine::Sort::None, 100 };
	long connectionStatus{ Engine::testConnection() };
	std::vector<Engine::Book> books = Engine::search(sParams);

	ASSERT_EQ(connectionStatus, 200);
	ASSERT_EQ(books.size(), 100);
	EXPECT_EQ(books[0].author.empty(), false);
	EXPECT_EQ(books[0].language.empty(), false);
	EXPECT_EQ(books[0].title.empty(), false);
	EXPECT_NE(books[0].year, 0);
	EXPECT_EQ(books[0].link.empty(), false);
}

TEST(EngineTest, RandomSearch)
{
	Engine::SearchParams sParams{ {"J.R.R. Tolkien"}, {}, "The lord of the rings", 0, Engine::Sort::None, 100 };
	long connectionStatus{ Engine::testConnection() };
	Engine::Book book{ Engine::randomSearch(sParams) };

	ASSERT_EQ(connectionStatus, 200);
	EXPECT_EQ(book.author.empty(), false);
	EXPECT_EQ(book.language.empty(), false);
	EXPECT_EQ(book.title.empty(), false);
	EXPECT_NE(book.year, 0);
	EXPECT_EQ(book.link.empty(), false);
}


TEST(EngineTest, GetSearchParams_OkGuest)
{
	Engine::SearchParams sParams;

	ASSERT_EQ(Engine::getSearchParams("", sParams), Engine::ResponseCode::Ok);
	EXPECT_EQ(sParams.authors.size(), 0);
	EXPECT_EQ(sParams.langs.empty(), true);
	EXPECT_EQ(sParams.title.empty(), true);
	EXPECT_EQ(sParams.year, 0);
	EXPECT_EQ(sParams.resListSize, 10);
}

TEST(EngineTest, GetSearchParams_OkUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);
	Engine::SearchParams sParams;

	ASSERT_EQ(Engine::getSearchParams("user", sParams), Engine::ResponseCode::Ok);
	EXPECT_EQ(sParams.authors.size(), 0);
	EXPECT_EQ(sParams.langs.size(), 1);
	EXPECT_EQ(sParams.langs[0], "en");
	EXPECT_EQ(sParams.title, "The lord of the rings");
	EXPECT_EQ(sParams.year, 0);
	EXPECT_EQ(sParams.resListSize, 10);
}

TEST(EngineTest, GetSearchParams_NoUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	Engine::SearchParams sParams;

	ASSERT_EQ(Engine::getSearchParams("user", sParams), Engine::ResponseCode::NoUser);
	EXPECT_EQ(sParams.authors.size(), 0);
	EXPECT_EQ(sParams.langs.size(), 0);
	EXPECT_EQ(sParams.title.empty(), true);
	EXPECT_EQ(sParams.year, 0);
	EXPECT_EQ(sParams.resListSize, 10);
}


TEST(EngineTest, SaveSearchParams_OkGuest)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	Engine::SearchParams sParams{ {"Author"}, {"Language"}, "Title", 2000, Engine::Sort::None, 100 };

	ASSERT_EQ(Engine::saveSearchParams("", sParams), Engine::ResponseCode::Ok);
}

TEST(EngineTest, SaveSearchParams_OkUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);
	Engine::SearchParams sParams{ {"Author"}, {"Language"}, "Title", 2000, Engine::Sort::None, 100 };

	ASSERT_EQ(Engine::saveSearchParams("user", sParams), Engine::ResponseCode::Ok);
}

TEST(EngineTest, SaveSearchParams_NoUser)
{
	Engine::FILENAME = "test.txt";
	Engine::recreateFile();
	Engine::SearchParams sParams{ {"Author"}, {"Language"}, "Title", 2000, Engine::Sort::None, 100 };

	ASSERT_EQ(Engine::saveSearchParams("user", sParams), Engine::ResponseCode::NoUser);
}