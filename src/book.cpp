#include "book.h"

Book::Book(std::vector<std::string> author, std::vector<std::string> language, std::string title, std::string link, size_t year)
	: author(author), language(language), title(title), year(year), link(link) {}

Book::Book(const Book& book)
	: author(book.author), language(book.language), title(book.title), year(book.year), link(book.link) {}
