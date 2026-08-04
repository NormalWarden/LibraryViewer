#pragma once

#include <string_view>

namespace JSONTemplates
{
	inline constexpr std::string_view fullTemplate = R"({
	"users" : [
	{
		"username" : "name",
		"searchParams" : {
			"author" : "Any",
			"language" : [
				"en"
			],
			"title" : "The lord of the rings",
			"year" : 0,
			"yearSearch" : "After",
			"sort" : "None",
			"resListSize" : 10
		},
		"recentlyBooks" : [
			{
				"author" : "J.R.R. Tolkien",
				"title" : "The Lord of the Rings",
				"language" : "en",
				"firstPublishYear" : 1954,
				"link" : "https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings?edition=key%3A/books/OL51694024M"
			}
		],
		"favoriteBooks" : [
			{
				"author" : "J.R.R. Tolkien",
				"title" : "The Lord of the Rings",
				"language" : "en",
				"firstPublishYear" : 1954,
				"link" : "https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings?edition=key%3A/books/OL51694024M"
			}
		]
	}
	]
})";
	inline constexpr std::string_view userTemplate{ R"({"username" : "",
		"searchParams" : {
			"author" : "",
			"language" : [
				"en"
			],
			"title" : "",
			"year" : 0,
			"yearSearch" : "After",
			"sort" : "None",
			"resListSize" : 10
		},
		"recentlyBooks" : [
		],
		"favoriteBooks" : [
		]
	})" };
}