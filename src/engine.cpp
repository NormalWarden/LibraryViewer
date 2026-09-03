#include "engine.h"

using json = nlohmann::json;

long Engine::testConnection()
{
	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" },
		cpr::Parameters{ {"q", "the+lord+of+the+rings"} },
		cpr::VerifySsl{ false });
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

std::vector<std::string> Engine::getUsers()
{
	std::vector<std::string> users;
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

Engine::ResponseCode Engine::createUser(const std::string& username)
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

std::vector<Engine::Book> Engine::getFavoriteBooks(std::string_view username)
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
			for (int j{}; j < userdata["users"][i]["favoriteBooks"].size(); ++j)
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

Engine::ResponseCode Engine::addFavoriteBook(std::string_view username, const Book& book)
{
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return transferRes;
	}

	if (username.empty())
	{
		return ResponseCode::GuestFavoriteOrRecentlyBook;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
			if (userdata["users"][i]["favoriteBooks"].size() == 5)
			{
				return ResponseCode::MaxCountOfFavoriteBooks;
			}
			userdata["users"][i]["favoriteBooks"].push_back({
				{"author", book.author},
				{"language", book.language},
				{"title", book.title},
				{"link", book.link},
				{"year", book.year}});
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
		return ResponseCode::GuestFavoriteOrRecentlyBook;
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

std::vector<Engine::Book> Engine::getRecentlyBooks(std::string_view username)
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
			for (int j{}; j < userdata["users"][i]["recentlyBooks"].size(); ++j)
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

Engine::ResponseCode Engine::addRecentlyBook(std::string_view username, const Book& book)
{
	json userdata{};
	if (ResponseCode transferRes{ fileToJSON(userdata) }; transferRes != ResponseCode::Ok)
	{
		return transferRes;
	}

	if (username.empty())
	{
		return ResponseCode::GuestFavoriteOrRecentlyBook;
	}
	for (int i{}; i < userdata["users"].size(); ++i)
	{
		if (userdata["users"][i]["username"].get<std::string_view>() == username)
		{
			userdata["users"][i]["recentlyBooks"].push_back({
				{"author", book.author},
				{"language", book.language},
				{"title", book.title},
				{"year", book.year},
				{"link", book.link}});
			while (userdata["users"][i]["recentlyBooks"].size() >= 6)
			{
				userdata["users"][i]["recentlyBooks"].erase(0);
			}
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
		return ResponseCode::GuestFavoriteOrRecentlyBook;
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

std::string Engine::getBookDescription(std::string_view link)
{
	if (link.empty())
	{
		return "";
	}
	
	cpr::Response r = cpr::Get(cpr::Url{ link },
		cpr::VerifySsl{ false });
	if (r.status_code != 200) // Bad link
	{
		return "";
	}
	std::string description;
	if (r.text.find("<ol-read-more class=\"book-description\" more-text=\"Read More\" less-text=\"Read Less\">") != std::string::npos &&
		r.text.find("</ol-read-more>") != std::string::npos)
	{
		description = std::string(begin(r.text) + 83 + r.text.find("<ol-read-more class=\"book-description\" more-text=\"Read More\" less-text=\"Read Less\">"),
			begin(r.text) + r.text.find("</ol-read-more>")); // Entire description with HTML tags. 83 - count of desired string in find
	}	
	if (description.find("<p>") != std::string::npos)
	{
		description = std::string(begin(description) + 3 + description.find("<p>"), end(description)); // Description without first spaces and <p>. 3 - count of string "<p>"
	}
	if (description.find("<hr/>") != std::string::npos)
	{
		description = std::string(begin(description), begin(description) + description.find("<hr/>")); // Description without information about book containing (just some additional info
	}
	if (description.find("</p>") != std::string::npos)
	{
		description = std::string(begin(description), begin(description) + description.rfind("</p>")); // Description without last spaces and </p>
	}

	for (size_t pos{ description.find("</p>") }; pos != std::string::npos; pos = description.find("</p>"))
	{
		description.erase(pos, 8); // 8 - count of string "</p>\n<p>"
		pos = description.find("</p>");
	}

	return description;
}

std::vector<Engine::Book> Engine::search(const SearchParams& sParams)
{
	std::vector<Book> books;
	cpr::Parameters cprParams;
	if (!sParams.title.empty())
	{
		cprParams.Add({ "q", transformStrToURL(sParams.title) });
	}
	else
	{
		return books; // To search books without title use random search
	}
	if (sParams.year != 0)
	{
		cprParams.Add({ "first_publish_year", std::to_string(sParams.year) });
	}
	if (!sParams.authors.empty())
	{
		for (const auto& author : sParams.authors)
		{
			cprParams.Add({ "author", author });
		}
	}
	if (!sParams.langs.empty())
	{
		for (const auto& lang : sParams.langs)
		{
			if (LangStorage::language.find(lang) != LangStorage::language.end())
			{
				cprParams.Add({ "language", LangStorage::language[lang] });
			}
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

	cpr::Response r = cpr::Get(cpr::Url{"https://openlibrary.org/search.json"},
		cprParams,
		cpr::VerifySsl{ false });
	if (r.status_code != 200) // Bad params or connection
	{
		return books;
	}
	json response{ json::parse(r.text) };
	Book book{};
	for (int i{}; i < sParams.resListSize; ++i)
	{
		if (i < response["docs"].size())
		{
			if (response["docs"][i].contains("author_name"))
			{
				book.author = response["docs"][i]["author_name"].get<std::vector<std::string>>();
			}
			if (response["docs"][i].contains("language"))
			{
				book.language = response["docs"][i]["language"].get<std::vector<std::string>>();
			}
			if (response["docs"][i].contains("title"))
			{
				book.title = response["docs"][i]["title"].get<std::string>();
				if (response["docs"][i].contains("key"))
				{
					book.link = makeLinkFromResponse(response, i);
				}
			}
			if (response["docs"][i].contains("first_publish_year"))
			{
				book.year = response["docs"][i]["first_publish_year"].get<size_t>();
			}
			books.push_back(book);
		}
	}
	return books;
}

Engine::Book Engine::randomSearch(const SearchParams& sParams)
{	
	cpr::Parameters cprParams;
	cprParams.Add({ "q", "*:*" }); // Book with any name (especially for the Apache Solr - search engine in openlibrary)
	cprParams.Add({ "sort", "random" });
	if (sParams.year != 0)
	{
		cprParams.Add({ "first_publish_year", std::to_string(sParams.year) });
	}
	if (!sParams.authors.empty())
	{
		for (const auto& author : sParams.authors)
		{
			cprParams.Add({ "author", author });
		}
	}
	if (!sParams.langs.empty())
	{
		for (const auto& lang : sParams.langs)
		{
			if (LangStorage::language.find(lang) != LangStorage::language.end())
			{
				cprParams.Add({ "language", LangStorage::language[lang] });
			}
		}
	}

	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" },
		cprParams,
		cpr::VerifySsl{ false });
	if (r.status_code != 200) // Bad params or connection
	{
		return Book();
	}
	json response{ json::parse(r.text) };
	int num = randBookNumber(response["docs"].size());
	
	Book book{};
	if (response["docs"][num].contains("author_name"))
	{
		book.author = response["docs"][num]["author_name"].get<std::vector<std::string>>();
	}
	if (response["docs"][num].contains("language"))
	{
		book.language = response["docs"][num]["language"].get<std::vector<std::string>>();
	}
	if (response["docs"][num].contains("title"))
	{
		book.title = response["docs"][num]["title"].get<std::string>();
		if (response["docs"][num].contains("key"))
		{
			book.link = makeLinkFromResponse(response, num);
		}
	}
	if (response["docs"][num].contains("first_publish_year"))
	{
		book.year = response["docs"][num]["first_publish_year"].get<size_t>();
	}
	return book;
}

Engine::ResponseCode Engine::getSearchParams(std::string_view username, SearchParams& sParams)
{
	if (username.empty()) // guest
	{
		sParams.authors = {};
		sParams.langs = {};
		sParams.title = "";
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
	for (int i{}; i < userdata["users"].size(); ++i)
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

int Engine::randBookNumber(unsigned int maxNum)
{
	static std::random_device rd;
	static std::mt19937 rng{ rd() };
	std::uniform_int_distribution<std::mt19937::result_type> uid{ 0,maxNum }; // Site search response contains 100 books
	return (int)uid(rng);
}

Engine::ResponseCode Engine::isRandomBookEmpty(const Book& book)
{
	if (book.author.empty())
	{
		if (book.language.empty())
		{
			if (book.title.empty())
			{
				if (book.link.empty())
				{
					if (!book.year)
					{
						return ResponseCode::EmptyRandomBook;
					}
				}
			}
		}
	}
	return ResponseCode::Ok;
}

std::string Engine::transformStrToURL(std::string_view str)
{
	std::string url{ str };
	std::replace(begin(url), end(url), ' ', '+');
	return url;
}

std::string Engine::makeLinkFromResponse(const nlohmann::json& response, int bookNumber)
{
	std::string link{ "https://openlibrary.org" + response["docs"][bookNumber]["key"].get<std::string>() + "/" + response["docs"][bookNumber]["title"].get<std::string>() };
	std::replace(begin(link), end(link), ' ', '_');
	return link;
}
