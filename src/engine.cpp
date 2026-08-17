#include "engine.h"

long Engine::testConnection()
{
	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" });
	return r.status_code;
}

Engine::ResponseCode Engine::createUser(const std::string& username)
{
	json userdata{ fileToJSON() };
	// Checks
	if (username.empty())
	{
		return ResponseCode::EmptyUsername;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			return ResponseCode::CreatingIdenticalUser;
		}
	}

	// Saving
	userdata["users"].push_back(json::parse(JSONTemplates::userTemplate));
	userdata["users"].back()["username"] = username;
	return JSONToFile(userdata);
}

Engine::ResponseCode Engine::deleteUser(const std::string& username)
{
	json userdata{ fileToJSON() };

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			userdata["users"].erase(i);
			return JSONToFile(userdata);
		}
	}
	return ResponseCode::NoUser;
}

void Engine::chooseUser(std::string& username, const short choice)
{
	json userdata{ fileToJSON() };

	if (choice == 1)
		username.clear();
	else
		username = userdata["users"].at(choice)["username"];
}

std::vector<std::string_view> Engine::getUsers(const std::string& filename)
{
	json data{ fileToJSON() };
	std::vector<std::string_view> users;
	
	for (int i{}; i < data["users"].size(); ++i)
		users.push_back(data["users"][i]["username"]);
	return users;
}

std::vector<Book> Engine::getFavoriteBooks(const std::string& username)
{
	json userdata{ fileToJSON() };
	std::vector<Book> books;

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			if (userdata["users"][i]["favoriteBooks"].empty())
			{
				return books;
			}
			for (int j{}; j < userdata["users"][i]["favoriteBooks"].size(); ++i)
			{
				books.push_back(Book(userdata["users"][i]["favoriteBooks"][j]["author"],
					userdata["users"][i]["favoriteBooks"][j]["language"],
					userdata["users"][i]["favoriteBooks"][j]["title"],
					userdata["users"][i]["favoriteBooks"][j]["link"],
					userdata["users"][i]["favoriteBooks"][j]["year"]));
			}
			return books;
		}
	}

	return std::vector<Book>();
}

std::vector<Book> Engine::getRecentlyBooks(const std::string username)
{
	json userdata{ fileToJSON() };
	std::vector<Book> books;

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			if (userdata["users"][i]["recentlyBooks"].empty())
			{
				return books;
			}
			for (int j{}; j < userdata["users"][i]["recentlyBooks"].size(); ++i)
			{
				books.push_back(Book(userdata["users"][i]["recentlyBooks"][j]["author"],
					userdata["users"][i]["recentlyBooks"][j]["language"],
					userdata["users"][i]["recentlyBooks"][j]["title"],
					userdata["users"][i]["recentlyBooks"][j]["link"],
					userdata["users"][i]["recentlyBooks"][j]["year"]));
			}
			return books;
		}
	}

	return std::vector<Book>();
}

std::vector<Book> Engine::search(const SearchParams& sParams)
{
	cpr::Parameters cprParams;
	if (!sParams.title.empty())
	{
		cprParams.Add({ "q",
			[](const std::string& title)->std::string
			{
				std::string correctTitle = title;
				std::replace(begin(correctTitle), end(correctTitle), ' ', '+');
				return correctTitle;
			}(sParams.title) });
	}
	if (sParams.year != 0)
		cprParams.Add({ "publish_year", std::to_string(sParams.year) });
	if (!sParams.author.empty())
		cprParams.Add({ "author", sParams.author });
	if (!sParams.langs.empty())
		for (auto lang : sParams.langs)
			cprParams.Add({ "language", lang });
	if (sParams.sort != Sort::None)
	{
		switch (sParams.sort)
		{
		case Sort::Editions:
			cprParams.Add({ "sort", "editions" });
			break;
		case Sort::Old:
			cprParams.Add({ "sort", "old" });
			break;
		case Sort::New:
			cprParams.Add({ "sort", "new" });
			break;
		case Sort::Rating:
			cprParams.Add({ "sort", "rating" });
			break;
		}
	}

	cpr::Response r = cpr::Get(cpr::Url{"https://openlibrary.org/search.json"}, cprParams);
	json j = json::parse(r.text);
	std::vector<Book> books;
	for (int el{ 0 }; el < sParams.resListSize; ++el)
	{
		books.push_back(Book(j["docs"][el]["author_name"],
			j["docs"][el]["language"],
			j["docs"][el]["title"],
			[](const json& j, const int& iter)->std::string
			{
				std::string link{ "https://openlibrary.org" + j["docs"][iter]["key"].get<std::string>() + "/" + j["docs"][iter]["title"].get<std::string>() };
				std::replace(begin(link), end(link), ' ', '_');
				return link;
			}(j, el),
			j["docs"][el]["first_publish_year"]));
	}
	return books;
}

Book Engine::randomSearch(const SearchParams& sParams)
{	
	cpr::Parameters cprParams;
	if (sParams.year != 0)
		cprParams.Add({ "publish_year", std::to_string(sParams.year) });
	if (!sParams.author.empty())
		cprParams.Add({ "author", sParams.author });
	if (!sParams.langs.empty())
		for (auto lang : sParams.langs)
			cprParams.Add({ "language", lang });

	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" }, cprParams);
	json j = json::parse(r.text);
	std::vector<Book> books;
	for (int el{ 0 }; el < 100; ++el)
	{
		books.push_back(Book(j["docs"][el]["author_name"],
			j["docs"][el]["language"],
			j["docs"][el]["title"],
			[](const json& j, const int& iter)->std::string
			{
				std::string link{ "https://openlibrary.org" + j["docs"][iter]["key"].get<std::string>() + "/" + j["docs"][iter]["title"].get<std::string>() };
				std::replace(begin(link), end(link), ' ', '_');
				return link;
			}(j, el),
				j["docs"][el]["first_publish_year"]));
	}
	std::random_device rd;
	std::mt19937 rng{ rd() };
	std::uniform_int_distribution<std::mt19937::result_type> uid{ 0,99 };
	return books[uid(rng)];
}

Engine::ResponseCode Engine::getSearchParams(const std::string& username, SearchParams& sParams)
{
	if (username.empty()) // guest
	{
		sParams.author = "";
		sParams.langs = { "eng" };
		sParams.title = "the lord of the rings";
		sParams.year = 1954;
		sParams.sort = Sort::None;
		sParams.resListSize = 10;
		return ResponseCode::Ok;
	}

	json userdata{ Engine::fileToJSON() };

	if (userdata.empty())
		return ResponseCode::EmptyJSON;
	for (int i{}; i < userdata["users"]; ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			sParams.author = userdata["users"][i]["searchParams"]["author"];
			sParams.langs = userdata["users"][i]["searchParams"]["language"];
			sParams.title = userdata["users"][i]["searchParams"]["title"];
			sParams.year = userdata["users"][i]["searchParams"]["year"];
			sParams.sort = userdata["users"][i]["searchParams"]["sort"];
			sParams.resListSize = userdata["users"][i]["searchParams"]["resListSize"];
			return ResponseCode::Ok;
		}
		return ResponseCode::NoUser;
	}
}

Engine::ResponseCode Engine::saveSearchParams(const std::string& username, const SearchParams& sParams)
{
	json userdata{ fileToJSON() };

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			userdata["users"][i]["searchParams"]["author"] = sParams.author;
			userdata["users"][i]["searchParams"]["language"] = sParams.langs;
			userdata["users"][i]["searchParams"]["title"] = sParams.title;
			userdata["users"][i]["searchParams"]["year"] = sParams.year;
			userdata["users"][i]["searchParams"]["sort"] = sParams.sort;
			userdata["users"][i]["searchParams"]["resListSize"];
			return JSONToFile(userdata);
		}
	}
	return ResponseCode::NoUser;
}

Engine::ResponseCode Engine::addFavoriteBook(const std::string& username, Book book)
{
	json userdata{ fileToJSON() };

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			userdata["users"][i]["favoriteBooks"].push_back({
				{"author", book.getAuthor()},
				{"language", book.getLanguage()},
				{"title", book.getTitle()},
				{"link", book.getLink()},
				{"year", book.getYear()}				
				});
			return JSONToFile(userdata);
		}
	}

	return ResponseCode::NoUser;
}

Engine::ResponseCode Engine::deleteFavoriteBook(const std::string& username, const short bookNumber)
{
	json userdata{ fileToJSON() };

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			userdata["users"][i]["favoriteBooks"].erase(bookNumber);
			return JSONToFile(userdata);
		}
	}

	return ResponseCode::NoUser;
}

Engine::ResponseCode Engine::addRecentlyBook(const std::string& username, Book book)
{
	json userdata{ fileToJSON() };

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			userdata["users"][i]["recentlyBooks"].push_back({
				{"author", book.getAuthor()},
				{"language", book.getLanguage()},
				{"title", book.getTitle()},
				{"year", book.getYear()},
				{"link", book.getLink()},
				});
			return JSONToFile(userdata);
		}
	}
	return ResponseCode::NoUser;
}

Engine::ResponseCode Engine::deleteRecentlyBook(const std::string& username, const short bookNumber)
{
	json userdata{ fileToJSON() };

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"] == username)
		{
			userdata["users"][i]["recentlyBooks"].erase(bookNumber);
			return JSONToFile(userdata);
		}
	}
	return ResponseCode::NoUser;
}

json Engine::fileToJSON(const std::string& filename)
{
	std::fstream file(filename);
	if (!file)
	{
		file.open(filename, std::ios::out); // create file
		file.close();
		file.open(filename);
	}

	json userdata;
	userdata = json::parse(file);
	file.close();
	return userdata;
}

Engine::ResponseCode Engine::JSONToFile(const json& userdata, const std::string& filename)
{
	std::fstream file(filename, std::ios::out | std::ios::trunc);
	if (!file.is_open())
		return ResponseCode::FailedFileUpdate;
	file << userdata.dump(4);
	if (!file.good())
	{
		file.close();
		return ResponseCode::FailedFileUpdate;
	}
	file.close();
	return ResponseCode::Ok;
}

Engine::ResponseCode Engine::recreateFile(const std::string& filename)
{
	std::fstream file{ filename };
	file.open(filename, std::ios::out | std::ios::trunc);
	if (!file.is_open())
		return ResponseCode::FailedFileUpdate;
	file << JSONTemplates::fullTemplate;
	if (!file.good())
	{
		file.close();
		return ResponseCode::FailedFileUpdate;
	}
	file.close();
	return ResponseCode::Ok;
}