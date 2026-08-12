#include "book.h"

Book::Book(std::vector<std::string> author, std::vector<std::string> language, std::string title, std::string link, size_t year)
	: author(author), language(language), title(title), year(year), link(link) {}

Book::Book(const Book& book)
	: author(book.author), language(book.language), title(book.title), year(book.year), link(book.link) {}

std::vector<std::string> Book::getAuthor() const
{
	return author;
}

std::vector<std::string> Book::getLanguage() const
{
	return language;
}

std::string Book::getTitle() const
{
	return title;
}

size_t Book::getYear() const
{
	return year;
}

std::string Book::getLink() const
{
	return link;
}
