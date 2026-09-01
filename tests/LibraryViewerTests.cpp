#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <string>
#include "book.h"
#include "engine.h"
#include "jsonTemplates.h"

std::string fileToString()
{
	std::string filedata, fileline;
	std::fstream file{ Engine::filename };
	std::getline(file, fileline);
	filedata = fileline;
	while (std::getline(file, fileline))
	{
		filedata += "\n" + fileline;
	}
	file.close();
	return filedata;
}

TEST(BookTest, VerifyAuthorGetter)
{
	Book book{ std::vector<std::string>{ "J.R.R. Tolkien"}, 
		std::vector<std::string>{"Afar"}, 
		"The Lord of the Rings", 
		"https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings", 
		1954 };
	
	ASSERT_EQ(std::vector<std::string>{"J.R.R. Tolkien"}, book.getAuthor());
}

TEST(BookTest, VerifyLanguageGetter)
{
	Book book{ std::vector<std::string>{ "J.R.R. Tolkien"},
		std::vector<std::string>{"Afar"},
		"The Lord of the Rings",
		"https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings",
		1954 };

	ASSERT_EQ(std::vector<std::string>{"Afar"}, book.getLanguage());
}

TEST(BookTest, VerifyTitleGetter)
{
	Book book{ std::vector<std::string>{ "J.R.R. Tolkien"},
		std::vector<std::string>{"Afar"},
		"The Lord of the Rings",
		"https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings",
		1954 };

	ASSERT_EQ("The Lord of the Rings", book.getTitle());
}

TEST(BookTest, VerifyLinkGetter)
{
	Book book{ std::vector<std::string>{ "J.R.R. Tolkien"},
		std::vector<std::string>{"Afar"},
		"The Lord of the Rings",
		"https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings",
		1954 };

	ASSERT_EQ("https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings", book.getLink());
}

TEST(BookTest, VerifyYearGetter)
{
	Book book{ std::vector<std::string>{ "J.R.R. Tolkien"},
		std::vector<std::string>{"Afar"},
		"The Lord of the Rings",
		"https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings",
		1954 };

	ASSERT_EQ(1954, book.getYear());
}


TEST(BookTest, VerifyAuthorSetter)
{
	Book book{};
	book.setAuthor(std::vector<std::string>{ "J.R.R. Tolkien"});

	ASSERT_EQ(std::vector<std::string>{"J.R.R. Tolkien"}, book.getAuthor());
}

TEST(BookTest, VerifyLanguageSetter)
{
	Book book{};
	book.setLanguage(std::vector<std::string>{"Afar"});

	ASSERT_EQ(std::vector<std::string>{"Afar"}, book.getLanguage());
}

TEST(BookTest, VerifyTitleSetter)
{
	Book book{};
	book.setTitle("The Lord of the Rings");

	ASSERT_EQ("The Lord of the Rings", book.getTitle());
}

TEST(BookTest, VerifyLinkSetter)
{
	Book book{};
	book.setLink("https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings");

	ASSERT_EQ("https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings", book.getLink());
}

TEST(BookTest, VerifyYearSetter)
{
	Book book{};
	book.setYear(1954);

	ASSERT_EQ(1954, book.getYear());
}


TEST(BookTest, VerifyCopyConstructor)
{
	Book book{ std::vector<std::string>{ "J.R.R. Tolkien"}, 
		std::vector<std::string>{"Afar"}, 
		"The Lord of the Rings", 
		"https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings", 
		1954 };
	Book bookCopy{ book };

	ASSERT_NE(&book, &bookCopy);
	EXPECT_EQ(std::vector<std::string>{"J.R.R. Tolkien"}, bookCopy.getAuthor());
	EXPECT_EQ(std::vector<std::string>{"Afar"}, bookCopy.getLanguage());
	EXPECT_EQ("The Lord of the Rings", bookCopy.getTitle());
	EXPECT_EQ("https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings", bookCopy.getLink());
	EXPECT_EQ(1954, bookCopy.getYear());
}

TEST(BookTest, VerifyOperatorAssignment)
{
	Book book{ std::vector<std::string>{ "J.R.R. Tolkien"}, 
		std::vector<std::string>{"Afar"},
		"The Lord of the Rings", 
		"https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings", 
		1954 };
	Book bookCopy = book;

	ASSERT_NE(&book, &bookCopy);
	EXPECT_EQ(std::vector<std::string>{"J.R.R. Tolkien"}, bookCopy.getAuthor());
	EXPECT_EQ(std::vector<std::string>{"Afar"}, bookCopy.getLanguage());
	EXPECT_EQ("The Lord of the Rings", bookCopy.getTitle());
	EXPECT_EQ("https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings", bookCopy.getLink());
	EXPECT_EQ(1954, bookCopy.getYear());
}

TEST(EngineTest, TestConnection)
{
	ASSERT_EQ(Engine::testConnection(), 200);
}


TEST(EngineTest, RecreateFile_Ok)
{
	Engine::filename = "test.txt";

	ASSERT_EQ(Engine::recreateFile(), Engine::ResponseCode::Ok);
	ASSERT_EQ(fileToString(), JSONTemplates::startTemplate);
}

TEST(EngineTest, RecreateFile_FailedFileOpen)
{
	Engine::filename = "test/test.txt";
	// fstream with ios::out modifier can't create a folder but only a file
	ASSERT_EQ(Engine::recreateFile(), Engine::ResponseCode::FailedFileOpen);
}


TEST(EngineTest, FileToJSON_Ok)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	json filedata, templateData;
	templateData = json::parse(JSONTemplates::startTemplate);

	ASSERT_EQ(Engine::fileToJSON(filedata), Engine::ResponseCode::Ok);
	ASSERT_EQ(filedata, templateData);
}

TEST(EngineTest, FileToJSON_FailedFileOpen)
{
	Engine::filename = "test/test.txt";
	json userdata;
	// fstream with ios::out modifier can't create a folder but only a file
	ASSERT_EQ(Engine::fileToJSON(userdata), Engine::ResponseCode::FailedFileOpen);
}


TEST(EngineTest, JSONToFile_Ok)
{
	Engine::filename = "test.txt";
	json userdata{ JSONTemplates::startTemplate };

	ASSERT_EQ(Engine::JSONToFile(userdata), Engine::ResponseCode::Ok);
	ASSERT_EQ(fileToString(), userdata.dump(4));
}

TEST(EngineTest, JSONToFile_FailedFileOpen)
{
	Engine::filename = "test/test.txt";
	json userdata{ JSONTemplates::startTemplate };
	// fstream with ios::out modifier can't create a folder but only a file
	ASSERT_EQ(Engine::JSONToFile(userdata), Engine::ResponseCode::FailedFileOpen);
}


TEST(EngineTest, GetUsers_Success)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::getUsers().size(), 1); // only guest
}

TEST(EngineTest, GetUsers_FailedOpenFile)
{
	Engine::filename = "test/test.txt";
	ASSERT_EQ(Engine::getUsers().size(), 0);
}


TEST(EngineTest, CreateUser_Ok)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	std::string username = "new";

	ASSERT_EQ(Engine::createUser(username), Engine::ResponseCode::Ok);
}

TEST(EngineTest, CreateUser_CreatingIdenticalUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);

	ASSERT_EQ(Engine::createUser(username), Engine::ResponseCode::CreatingIdenticalUser);
	ASSERT_EQ(username, "");
}

TEST(EngineTest, CreateUser_EmptyUsername)
{
	Engine::filename = "test.txt";
	std::string username = "";

	ASSERT_EQ(Engine::createUser(username), Engine::ResponseCode::EmptyUsername);
}


TEST(EngineTest, ChooseUser_OkGuest)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	std::string username = "";

	ASSERT_EQ(Engine::chooseUser(username, 0), Engine::ResponseCode::Ok); // Chosen "guest"
	ASSERT_EQ(username, "");
}

TEST(EngineTest, ChooseUser_OkUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);

	ASSERT_EQ(Engine::chooseUser(username, 1), Engine::ResponseCode::Ok); // Chosen "new"
	ASSERT_EQ(username, "new");
}

TEST(EngineTest, ChooseUser_InvalidInputGuest)
{
	Engine::filename = "test.txt";
	std::string username = "";

	ASSERT_EQ(Engine::chooseUser(username, -1), Engine::ResponseCode::InvalidInput);
}

TEST(EngineTest, ChooseUser_InvalidInputUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);

	ASSERT_EQ(Engine::chooseUser(username, -1), Engine::ResponseCode::InvalidInput);
}


TEST(EngineTest, DeleteUser_Ok)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);

	ASSERT_EQ(Engine::deleteUser(username), Engine::ResponseCode::Ok);
	ASSERT_EQ(username, "");
}

TEST(EngineTest, DeleteUser_NoUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	std::string username = "new";

	ASSERT_EQ(Engine::deleteUser(username), Engine::ResponseCode::NoUser);
	ASSERT_EQ(username, "");
}

TEST(EngineTest, DeleteUser_EmptyUsername)
{
	Engine::filename = "test.txt";
	std::string username = "";

	ASSERT_EQ(Engine::deleteUser(username), Engine::ResponseCode::EmptyUsername);
}


TEST(EngineTest, GetFavoriteBooks_OkUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);

	ASSERT_EQ(Engine::getFavoriteBooks("user").size(), 1);
}

TEST(EngineTest, GetFavoriteBooks_GuestFavoriteBook)
{
	Engine::filename = "test.txt";

	ASSERT_EQ(Engine::getFavoriteBooks("").size(), 0); // guest has 0 favorite books
}

TEST(EngineTest, GetFavoriteBooks_NoUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::getFavoriteBooks("user").size(), 0);
}


TEST(EngineTest, AddFavoriteBook_Ok)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);
	Book favoriteBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };
	std::vector<Book> booksVTemplate{ favoriteBook };

	ASSERT_EQ(Engine::addFavoriteBook(username, favoriteBook), Engine::ResponseCode::Ok);
	EXPECT_EQ(Engine::getFavoriteBooks(username).size(), 1);
	EXPECT_EQ(Engine::getFavoriteBooks(username)[0].getAuthor(), favoriteBook.getAuthor());
	EXPECT_EQ(Engine::getFavoriteBooks(username)[0].getLanguage(), favoriteBook.getLanguage());
	EXPECT_EQ(Engine::getFavoriteBooks(username)[0].getTitle(), favoriteBook.getTitle());
	EXPECT_EQ(Engine::getFavoriteBooks(username)[0].getLink(), favoriteBook.getLink());
	EXPECT_EQ(Engine::getFavoriteBooks(username)[0].getYear(), favoriteBook.getYear());
}

TEST(EngineTest, AddFavoriteBook_GuestFavoriteOrRecentlyBook)
{
	Engine::filename = "test.txt";
	Book favoriteBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };

	ASSERT_EQ(Engine::addFavoriteBook("", favoriteBook), Engine::ResponseCode::GuestFavoriteOrRecentlyBook);
	ASSERT_EQ(Engine::getFavoriteBooks("").size(), 0);
}

TEST(EngineTest, AddFavoriteBook_NoUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	Book favoriteBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };

	ASSERT_EQ(Engine::addFavoriteBook("user", favoriteBook), Engine::ResponseCode::NoUser);
	ASSERT_EQ(Engine::getFavoriteBooks("user").size(), 0);
}


TEST(EngineTest, DeleteFavoriteBook_Ok)
{
	Engine::filename = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);

	ASSERT_EQ(Engine::deleteFavoriteBook("user", 0), Engine::ResponseCode::Ok);
	ASSERT_EQ(Engine::getFavoriteBooks("user").size(), 0);
}

TEST(EngineTest, DeleteFavoriteBook_GuestFavoriteBook)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::deleteFavoriteBook("", 0), Engine::ResponseCode::GuestFavoriteOrRecentlyBook);
}

TEST(EngineTest, DeleteFavoriteBook_NoUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::deleteFavoriteBook("user", 0), Engine::ResponseCode::NoUser);
	ASSERT_EQ(Engine::getFavoriteBooks("user").size(), 0);
}


TEST(EngineTest, GetRecentlyBooks_OkUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);

	ASSERT_EQ(Engine::getRecentlyBooks("user").size(), 1);
}

TEST(EngineTest, GetRecentlyBooks_GuestFavoriteBook)
{
	Engine::filename = "test.txt";

	ASSERT_EQ(Engine::getRecentlyBooks("").size(), 0); // guest has 0 recently books
}

TEST(EngineTest, GetRecentlyBooks_NoUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::getRecentlyBooks("user").size(), 0);
}


TEST(EngineTest, AddRecentlyBook_Ok)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	std::string username = "new";
	Engine::createUser(username);
	Book recentlyBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };

	ASSERT_EQ(Engine::addRecentlyBook(username, recentlyBook), Engine::ResponseCode::Ok);
	EXPECT_EQ(Engine::getRecentlyBooks(username).size(), 1);
	EXPECT_EQ(Engine::getRecentlyBooks(username)[0].getAuthor(), recentlyBook.getAuthor());
	EXPECT_EQ(Engine::getRecentlyBooks(username)[0].getLanguage(), recentlyBook.getLanguage());
	EXPECT_EQ(Engine::getRecentlyBooks(username)[0].getTitle(), recentlyBook.getTitle());
	EXPECT_EQ(Engine::getRecentlyBooks(username)[0].getLink(), recentlyBook.getLink());
	EXPECT_EQ(Engine::getRecentlyBooks(username)[0].getYear(), recentlyBook.getYear());
}

TEST(EngineTest, AddRecentlyBook_GuestFavoriteOrRecentlyBook)
{
	Engine::filename = "test.txt";
	Book recentlyBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };

	ASSERT_EQ(Engine::addRecentlyBook("", recentlyBook), Engine::ResponseCode::GuestFavoriteOrRecentlyBook);
	ASSERT_EQ(Engine::getRecentlyBooks("").size(), 0);
}

TEST(EngineTest, AddRecentlyBook_NoUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	Book recentlyBook{ {"Author"}, {"Language"}, "Title", "Link", 2000 };

	ASSERT_EQ(Engine::addRecentlyBook("user", recentlyBook), Engine::ResponseCode::NoUser);
	ASSERT_EQ(Engine::getRecentlyBooks("user").size(), 0);
}


TEST(EngineTest, DeleteRecentlyBook_Ok)
{
	Engine::filename = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);

	ASSERT_EQ(Engine::deleteRecentlyBook("user", 0), Engine::ResponseCode::Ok);
	ASSERT_EQ(Engine::getRecentlyBooks("user").size(), 0);
}

TEST(EngineTest, DeleteRecentlyBook_GuestFavoriteBook)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::deleteRecentlyBook("", 0), Engine::ResponseCode::GuestFavoriteOrRecentlyBook);
}

TEST(EngineTest, DeleteRecentlyBook_NoUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();

	ASSERT_EQ(Engine::deleteRecentlyBook("user", 0), Engine::ResponseCode::NoUser);
	ASSERT_EQ(Engine::getRecentlyBooks("user").size(), 0);
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
	std::vector<Book> books = Engine::search(sParams);

	EXPECT_EQ(connectionStatus, 200);
	ASSERT_EQ(books.size(), 100);
	ASSERT_EQ(books[0].getAuthor().empty(), false);
	ASSERT_EQ(books[0].getLanguage().empty(), false);
	ASSERT_EQ(books[0].getTitle().empty(), false);
	ASSERT_NE(books[0].getYear(), 0);
	ASSERT_EQ(books[0].getLink().empty(), false);
}

TEST(EngineTest, RandomSearch)
{
	Engine::SearchParams sParams{ {"J.R.R. Tolkien"}, {}, "The lord of the rings", 0, Engine::Sort::None, 100 };
	long connectionStatus{ Engine::testConnection() };
	Book book{ Engine::randomSearch(sParams) };

	EXPECT_EQ(connectionStatus, 200);
	ASSERT_EQ(book.getAuthor().empty(), false);
	ASSERT_EQ(book.getLanguage().empty(), false);
	ASSERT_EQ(book.getTitle().empty(), false);
	ASSERT_NE(book.getYear(), 0);
	ASSERT_EQ(book.getLink().empty(), false);
}


TEST(EngineTest, GetSearchParams_OkGuest)
{
	Engine::SearchParams sParams;

	ASSERT_EQ(Engine::getSearchParams("", sParams), Engine::ResponseCode::Ok);
	ASSERT_EQ(sParams.authors.size(), 0);
	ASSERT_EQ(sParams.langs.size(), 1);
	ASSERT_EQ(sParams.langs[0], "en");
	ASSERT_EQ(sParams.title, "The lord of the rings");
	ASSERT_EQ(sParams.year, 0);
	ASSERT_EQ(sParams.resListSize, 10);
}

TEST(EngineTest, GetSearchParams_OkUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);
	Engine::SearchParams sParams;

	ASSERT_EQ(Engine::getSearchParams("user", sParams), Engine::ResponseCode::Ok);
	ASSERT_EQ(sParams.authors.size(), 0);
	ASSERT_EQ(sParams.langs.size(), 1);
	ASSERT_EQ(sParams.langs[0], "en");
	ASSERT_EQ(sParams.title, "The lord of the rings");
	ASSERT_EQ(sParams.year, 0);
	ASSERT_EQ(sParams.resListSize, 10);
}

TEST(EngineTest, GetSearchParams_NoUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	Engine::SearchParams sParams;

	ASSERT_EQ(Engine::getSearchParams("user", sParams), Engine::ResponseCode::NoUser);
	ASSERT_EQ(sParams.authors.size(), 0);
	ASSERT_EQ(sParams.langs.size(), 0);
	ASSERT_EQ(sParams.title.empty(), true);
	ASSERT_EQ(sParams.year, 0);
	ASSERT_EQ(sParams.resListSize, 10);
}


TEST(EngineTest, SaveSearchParams_OkGuest)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	Engine::SearchParams sParams{ {"Author"}, {"Language"}, "Title", 2000, Engine::Sort::None, 100 };

	ASSERT_EQ(Engine::saveSearchParams("", sParams), Engine::ResponseCode::Ok);
}

TEST(EngineTest, SaveSearchParams_OkUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile(JSONTemplates::fullTemplate);
	Engine::SearchParams sParams{ {"Author"}, {"Language"}, "Title", 2000, Engine::Sort::None, 100 };

	ASSERT_EQ(Engine::saveSearchParams("user", sParams), Engine::ResponseCode::Ok);
}

TEST(EngineTest, SaveSearchParams_NoUser)
{
	Engine::filename = "test.txt";
	Engine::recreateFile();
	Engine::SearchParams sParams{ {"Author"}, {"Language"}, "Title", 2000, Engine::Sort::None, 100 };

	ASSERT_EQ(Engine::saveSearchParams("user", sParams), Engine::ResponseCode::NoUser);
}