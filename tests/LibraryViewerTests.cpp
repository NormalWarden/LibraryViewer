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
	while (std::getline(file, fileline))
	{
		filedata += fileline;
	}
	file.close();
	return filedata;
}

TEST(BookTest, VerifyGettersAfterFullInit)
{
	Book book{ std::vector<std::string>{ "J.R.R. Tolkien"}, 
		std::vector<std::string>{"Afar"}, 
		"The Lord of the Rings", 
		"https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings", 
		1954 };
	
	EXPECT_EQ(std::vector<std::string>{"J.R.R. Tolkien"}, book.getAuthor());
	EXPECT_EQ(std::vector<std::string>{"Afar"}, book.getLanguage());
	EXPECT_EQ("The Lord of the Rings", book.getTitle());
	EXPECT_EQ("https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings", book.getLink());
	EXPECT_EQ(1954, book.getYear());
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
	ASSERT_EQ(fileToString, JSONTemplates::fullTemplate);
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
	json userdata;

	ASSERT_EQ(Engine::fileToJSON(userdata), Engine::ResponseCode::Ok);
	ASSERT_EQ(fileToString(), userdata.dump(4));
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
	json userdata{ JSONTemplates::fullTemplate };

	ASSERT_EQ(Engine::JSONToFile(userdata), Engine::ResponseCode::Ok);
	ASSERT_EQ(fileToString(), userdata.dump(4));
}

TEST(EngineTest, JSONToFile_FailedFileOpen)
{
	Engine::filename = "test/test.txt";
	json userdata{ JSONTemplates::fullTemplate };
	// fstream with ios::out modifier can't create a folder but only a file
	ASSERT_EQ(Engine::JSONToFile(userdata), Engine::ResponseCode::FailedFileOpen);
}

TEST(EngineTest, GetUsers_Success)
{
	Engine::filename = "test.txt";
	ASSERT_EQ(Engine::getUsers().size(), 1);
}

TEST(EngineTest, GetUsers_FailedOpenFile)
{
	Engine::filename = "test/test.txt";
	ASSERT_EQ(Engine::getUsers().size(), 0);
}

TEST(EngineTest, CreateUser_Ok)
{
	Engine::filename = "test.txt";
	std::string username = "new";
	ASSERT_EQ(Engine::createUser(username), Engine::ResponseCode::Ok);
}

TEST(EngineTest, CreateUser_CreatingIdenticalUser)
{
	Engine::filename = "test.txt";
	std::string username = "new";
	ASSERT_EQ(Engine::createUser(username), Engine::ResponseCode::CreatingIdenticalUser);
	ASSERT_EQ(username, "");
}

TEST(EngineTest, CreateUser_EmptyUsername)
{
	Engine::filename = "test.txt";
	std::string username = "";
	ASSERT_EQ(Engine::createUser(username), Engine::ResponseCode::EmptyUsername);
}

TEST(EngineTest, DeleteUser_Ok)
{
	Engine::filename = "test.txt";
	std::string username = "new";
	ASSERT_EQ(Engine::deleteUser(username), Engine::ResponseCode::Ok);
	ASSERT_EQ(username, "");
}

TEST(EngineTest, DeleteUser_NoUser)
{
	Engine::filename = "test.txt";
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