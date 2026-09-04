#pragma once

namespace Engine
{
	inline int randBookNumber(unsigned int maxNum) // 0...maxNum
	{
		static std::random_device rd;
		static std::mt19937 rng{ rd() };
		std::uniform_int_distribution<std::mt19937::result_type> uid{ 0,maxNum }; // Site search response contains 100 books
		return (int)uid(rng);
	}

	inline ResponseCode isRandomBookEmpty(const Book& book)
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

	inline std::string transformStrToURL(std::string_view str) // "first second third" -> first+second+third
	{
		std::string url{ str };
		std::replace(begin(url), end(url), ' ', '+');
		return url;
	}

	inline std::string makeLinkFromResponse(const nlohmann::json& response, int bookNumber) // Site URL + book key in response + title
	{
		std::string link{ "https://openlibrary.org" + response["docs"][bookNumber]["key"].get<std::string>() + "/" + response["docs"][bookNumber]["title"].get<std::string>() };
		std::replace(begin(link), end(link), ' ', '_');
		return link;
	}
}