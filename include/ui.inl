#pragma once

namespace UI
{
	inline std::string authorsVecToStr(const std::vector<std::string>& authorsV) // { "Mark Twain", "Charles Neider" } -> "Mark Twain, Charles Neider"
	{
		std::string authorsS{ authorsV[0] };
		if (authorsV.size() == 1)
		{
			return authorsS;
		}
		for (int author{ 1 }; author < authorsV.size(); ++author)
		{
			authorsS += ", " + authorsV[author];
		}
		return authorsS;
	}

	inline std::string languagesVecToStr(const std::vector<std::string>& langsV) // { "english", "spanish" } -> "english, spanish"
	{
		std::string langsS{ langsV[0] };
		if (langsV.size() == 1)
		{
			return langsS;
		}
		for (int author{ 1 }; author < langsV.size(); ++author)
		{
			langsS += ", " + langsV[author];
		}
		return langsS;
	}

	inline std::string sortModeToStr(const Engine::Sort& sort) // Engine::Sort::Mode -> "mode"
	{
		switch (sort)
		{
		case Engine::Sort::None:
			return "relevant";
		case Engine::Sort::Editions:
			return "count of editions";
		case Engine::Sort::Old:
			return "old";
		case Engine::Sort::New:
			return "new";
		case Engine::Sort::Rating:
			return "rating";
		default:
			return "relevant";
		}
	}
}