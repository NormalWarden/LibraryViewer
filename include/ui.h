#pragma once
#include <iostream>
#include <vector>
#include <string_view>
#include "engine.h"
#include "book.h"

namespace UI
{
	void firstMessage();

	void beforeConnection();
	void successfulConnection();
	void failedConnection(long status);

	void printOptions();
	short getUserChoice();

	void printChosenUser(std::string_view username);
	void printUsers(const std::vector<std::string_view>& users);

	void printBooks(const std::vector<Book>& books);

	void systemMessage(Engine::ResponseCode response);
}