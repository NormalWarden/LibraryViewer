#pragma once

#include <string>
#include <vector>

class Book 
{
	std::vector<std::string> author; 
	std::vector<std::string> language;
	std::string title; 
	size_t year; 
	std::string link;
public: 
	Book(std::vector<std::string> author = {}, std::vector<std::string> language = {}, std::string title = "", std::string link = "", size_t year = 0);
	Book(const Book& book);
	~Book() = default;
	//operator=
	std::vector<std::string> getAuthor() const;
	std::vector<std::string> getLanguage() const;
	std::string getTitle() const; 
	size_t getYear() const; 
	std::string getLink() const;
};