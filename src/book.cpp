#include "book.h"

Book::Book(std::vector<std::string> author, std::vector<std::string> language, std::string title, std::string link, size_t year)
	: author(author), language(language), title(title), year(year), link(link) {}

Book::Book(const Book& book)
	: author(book.author), language(book.language), title(book.title), year(book.year), link(book.link) {}

Book& Book::operator=(const Book& bookSrc)
{
	this->author = bookSrc.getAuthor();
	this->language = bookSrc.getAuthor();
	this->title = bookSrc.getTitle();
	this->year = bookSrc.getYear();
	this->link = bookSrc.getLink();
	return *this;
}
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
