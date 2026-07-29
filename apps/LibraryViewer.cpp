#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main()
{
	cpr::Response r = cpr::Get(cpr::Url{ "https://openlibrary.org/search.json" },
		cpr::Parameters{ {"q", "the+lord+of+the+rings"} });
	json j = json::parse(r.text);
	std::cout << j["docs"][0].dump(4);
	return 0;
}
