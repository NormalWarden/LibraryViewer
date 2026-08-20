#pragma once

#include <string_view>

namespace JSONTemplates
{
	inline constexpr std::string_view startTemplate = R"({
	"users" : [
		{
			"username" : "",
			"searchParams" : {
				"author" : "",
				"language" : [
				],
				"title" : "",
				"year" : 0,
				"sort" : 0,
				"resListSize" : 10
			},
			"recentlyBooks" : [
			],
			"favoriteBooks" : [
			]
		}
	]
})";
	inline constexpr std::string_view userTemplate{ R"({"username" : "",
		"searchParams" : {
			"author" : "",
			"language" : [
			],
			"title" : "",
			"year" : 0,
			"sort" : 0,
			"resListSize" : 10
		},
		"recentlyBooks" : [
		],
		"favoriteBooks" : [
		]
	})" };
	inline constexpr std::string_view fullTemplate = R"({
	"users" : [
	{
		"username" : "name",
		"searchParams" : {
			"author" : "",
			"language" : [
				"eng"
			],
			"title" : "The lord of the rings",
			"year" : 0,
			"sort" : "0",
			"resListSize" : 10
		},
		"recentlyBooks" : [
			{
				"author" : "J.R.R. Tolkien",
				"title" : "The Lord of the Rings",
				"language" : "en",
				"firstPublishYear" : 1954,
				"link" : "https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings"
			}
		],
		"favoriteBooks" : [
			{
				"author" : "J.R.R. Tolkien",
				"title" : "The Lord of the Rings",
				"language" : "en",
				"firstPublishYear" : 1954,
				"link" : "https://openlibrary.org/works/OL27448W/The_Lord_of_the_Rings"
			}
		]
	}
	]
})";
}