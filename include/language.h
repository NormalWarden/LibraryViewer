#pragma once

#include <string>
#include <unordered_map>

namespace Lang
{
	enum class Language
	{
		Afar
	};
	inline std::unordered_map<std::string, Language> language
	{
		{ "aar", Language::Afar }
	};
}