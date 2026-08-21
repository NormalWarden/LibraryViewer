#include "engine.h"

long Engine::testConnection()
{
	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" });
	return r.status_code;
}

Engine::ResponseCode Engine::recreateFile(std::string_view fileTemplate)
{
	std::fstream file{ filename, std::ios::out | std::ios::trunc };
	if (!file.is_open())
	{
		return ResponseCode::FailedFileOpen;
	}
	file << fileTemplate;
	if (!file.good())
	{
		file.close();
		return ResponseCode::FailedFileUpdate;
	}
	file.close();
	return ResponseCode::Ok;
}

Engine::ResponseCode Engine::fileToJSON(json& userdata)
{
	std::fstream file(filename);
	if (!file.is_open())
	{
		return ResponseCode::FailedFileOpen;
	}
	userdata = json::parse(file);
	file.close();
	return ResponseCode::Ok;
}

Engine::ResponseCode Engine::JSONToFile(const json& userdata)
{
	std::fstream file(filename, std::ios::out | std::ios::trunc);
	if (!file.is_open())
	{
		return ResponseCode::FailedFileOpen;
	}
	file << userdata.dump(4);
	if (!file.good())
	{
		file.close();
		return ResponseCode::FailedFileUpdate;
	}
	file.close();
	return ResponseCode::Ok;
}

std::vector<std::string_view> Engine::getUsers()
{
	std::vector<std::string_view> users;
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return users;
	}

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		users.push_back(userdata["users"][i]["username"]);
	}
	return users;
}

Engine::ResponseCode Engine::createUser(std::string& username)
{
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		username.clear();
		return transferRes;
	}

	if (username.empty())
	{
		return ResponseCode::EmptyUsername;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
			username.clear();
			return ResponseCode::CreatingIdenticalUser;
		}
	}

	userdata["users"].push_back(json::parse(JSONTemplates::userTemplate));
	userdata["users"].back()["username"] = username;
	return JSONToFile(userdata);
}

Engine::ResponseCode Engine::chooseUser(std::string& username, const short choice)
{
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return transferRes;
	}

	if ((choice < 0) || (choice > userdata["users"].size()))
	{
		return ResponseCode::InvalidInput;
	}
	if (choice == 0)
	{
		username.clear();
		return ResponseCode::Ok;
	}
	username = userdata["users"][choice]["username"];
	return ResponseCode::Ok;
}

Engine::ResponseCode Engine::deleteUser(std::string& username)
{
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return transferRes;
	}

	if (username.empty())
	{
		return ResponseCode::EmptyUsername;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
			userdata["users"].erase(i);
			username.clear();
			return JSONToFile(userdata);
		}
	}
	username.clear();
	return ResponseCode::NoUser;
}

std::vector<Book> Engine::getFavoriteBooks(std::string_view username)
{
	std::vector<Book> books;
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return books;
	}

	if (username.empty())
	{
		return books;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
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
	return books;
}

Engine::ResponseCode Engine::addFavoriteBook(std::string_view username, Book book)
{
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return transferRes;
	}

	if (username.empty())
	{
		return ResponseCode::GuestFavoriteBook;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
			userdata["users"][i]["favoriteBooks"].push_back({
				{"author", book.getAuthor()},
				{"language", book.getLanguage()},
				{"title", book.getTitle()},
				{"link", book.getLink()},
				{"year", book.getYear()}});
			return JSONToFile(userdata);
		}
	}
	return ResponseCode::NoUser;
}

Engine::ResponseCode Engine::deleteFavoriteBook(std::string_view username, const short bookNumber)
{
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return transferRes;
	}

	if (username.empty())
	{
		return ResponseCode::GuestFavoriteBook;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
			userdata["users"][i]["favoriteBooks"].erase(bookNumber);
			return JSONToFile(userdata);
		}
	}
	return ResponseCode::NoUser;
}

std::vector<Book> Engine::getRecentlyBooks(std::string_view username)
{
	std::vector<Book> books;
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return books;
	}

	if (username.empty())
	{
		return books;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
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
	return books;
}

Engine::ResponseCode Engine::addRecentlyBook(std::string_view username, Book book)
{
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return transferRes;
	}

	if (username.empty())
	{
		return ResponseCode::GuestFavoriteBook;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
			userdata["users"][i]["recentlyBooks"].push_back({
				{"author", book.getAuthor()},
				{"language", book.getLanguage()},
				{"title", book.getTitle()},
				{"year", book.getYear()},
				{"link", book.getLink()}});
			return JSONToFile(userdata);
		}
	}
	return ResponseCode::NoUser;
}

Engine::ResponseCode Engine::deleteRecentlyBook(std::string_view username, const short bookNumber)
{
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return transferRes;
	}

	if (username.empty())
	{
		return ResponseCode::GuestFavoriteBook;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
			userdata["users"][i]["recentlyBooks"].erase(bookNumber);
			return JSONToFile(userdata);
		}
	}
	return ResponseCode::NoUser;
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
	{
		cprParams.Add({ "publish_year", std::to_string(sParams.year) });
	}
	if (!sParams.authors.empty())
	{
		for (auto author : sParams.authors)
		{
			cprParams.Add({ "author", author });
		}
	}
	if (!sParams.langs.empty())
	{
		for (auto lang : sParams.langs)
		{
			cprParams.Add({ "language", lang });
		}
	}
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
	json response{ json::parse(r.text) };
	std::vector<Book> books;
	for (int i{}; i < sParams.resListSize; ++i)
	{
		books.push_back(Book(response["docs"][i]["author_name"],
			response["docs"][i]["language"],
			response["docs"][i]["title"],
			[](const json& response, const int& iter)->std::string
			{
				std::string link{ "https://openlibrary.org" + response["docs"][iter]["key"].get<std::string>() + "/" + response["docs"][iter]["title"].get<std::string>() };
				std::replace(begin(link), end(link), ' ', '_');
				return link;
			}(response, i),
			response["docs"][i]["first_publish_year"]));
	}
	return books;
}

Book Engine::randomSearch(const SearchParams& sParams)
{	
	cpr::Parameters cprParams;
	if (sParams.year != 0)
	{
		cprParams.Add({ "publish_year", std::to_string(sParams.year) });
	}
	if (!sParams.authors.empty())
	{
		for (auto author : sParams.authors)
		{
			cprParams.Add({ "author", author });
		}
	}
	if (!sParams.langs.empty())
	{
		for (auto lang : sParams.langs)
		{
			cprParams.Add({ "language", lang });
		}
	}

	// Search
	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" }, cprParams);
	json response{ json::parse(r.text) };
	
	// Random number
	std::random_device rd;
	std::mt19937 rng{ rd() };
	std::uniform_int_distribution<std::mt19937::result_type> uid{ 0,99 }; // Site search response contains 100 books
	size_t randNumber{ uid(rng) };

	return Book(response["docs"][randNumber]["author_name"],
		response["docs"][randNumber]["language"],
		response["docs"][randNumber]["title"],
		[](const json& response, const int& iter)->std::string
		{
			std::string link{ "https://openlibrary.org" + response["docs"][iter]["key"].get<std::string>() + "/" + response["docs"][iter]["title"].get<std::string>() };
			std::replace(begin(link), end(link), ' ', '_');
			return link;
		}(response, randNumber),
		response["docs"][randNumber]["first_publish_year"]);
}

Engine::ResponseCode Engine::getSearchParams(std::string_view username, SearchParams& sParams)
{
	if (username.empty()) // guest
	{
		sParams.authors = {};
		sParams.langs = { "en" };
		sParams.title = "The lord of the rings";
		sParams.year = 0;
		sParams.sort = Sort::None;
		sParams.resListSize = 10;
		return ResponseCode::Ok;
	}

	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return transferRes;
	}

	if (userdata.empty())
	{
		return ResponseCode::EmptyJSON;
	}
	for (int i{}; i < userdata["users"]; ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
			sParams.authors = userdata["users"][i]["searchParams"]["author"];
			sParams.langs = userdata["users"][i]["searchParams"]["language"];
			sParams.title = userdata["users"][i]["searchParams"]["title"];
			sParams.year = userdata["users"][i]["searchParams"]["year"];
			sParams.sort = userdata["users"][i]["searchParams"]["sort"];
			sParams.resListSize = userdata["users"][i]["searchParams"]["resListSize"];
			return ResponseCode::Ok;
		}
	}
	return ResponseCode::NoUser;
}

Engine::ResponseCode Engine::saveSearchParams(std::string_view username, const SearchParams& sParams)
{
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return transferRes;
	}

	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
			userdata["users"][i]["searchParams"]["author"] = sParams.authors;
			userdata["users"][i]["searchParams"]["language"] = sParams.langs;
			userdata["users"][i]["searchParams"]["title"] = sParams.title;
			userdata["users"][i]["searchParams"]["year"] = sParams.year;
			userdata["users"][i]["searchParams"]["sort"] = sParams.sort;
			userdata["users"][i]["searchParams"]["resListSize"] = sParams.resListSize;
			return JSONToFile(userdata);
		}
	}
	return ResponseCode::NoUser;
}