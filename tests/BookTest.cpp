#pragma once

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "book.h"


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