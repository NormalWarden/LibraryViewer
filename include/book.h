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
	std::string getAuthor();
	std::vector<std::string> getLanguage(); 
	std::string getTitle(); 
	size_t getYear(); 
};