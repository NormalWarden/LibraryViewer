#pragma once

#include <string>
#include <vector>

class Book 
{
	std::string author; 
	std::vector<std::string> language;
	std::string name; 
	size_t year; 
public: 
	std::string getAuthor(); // TODO: string_view?
	std::vector<std::string> getLanguage(); 
	std::string getName(); 
	size_t getYear(); 
};