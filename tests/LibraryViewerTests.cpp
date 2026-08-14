#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <string>
#include "book.h"
#include "engine.h"

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