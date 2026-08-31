#pragma once

#include <string>
#include <vector>

class Book 
{
	std::vector<std::string> author; 
	std::vector<std::string> language;
	std::string title; 
	std::string link;
	size_t year; 
public: 
	Book(std::vector<std::string> author = {}, std::vector<std::string> language = {}, std::string title = "", std::string link = "", size_t year = 0);
	Book(const Book& book);
	~Book() = default;
	Book& operator=(const Book& bookSrc);
	void setAuthor(const std::vector<std::string>& author);
	void setLanguage(const std::vector<std::string>& language);
	void setTitle(std::string_view title);
	void setLink(std::string_view link);
	void setYear(const size_t year);
	std::vector<std::string> getAuthor() const;
	std::vector<std::string> getLanguage() const;
	std::string getTitle() const; 
	std::string getLink() const;
	size_t getYear() const; 
};