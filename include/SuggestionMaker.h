#pragma once

#include <string>
#include <vector>

class SuggestionMaker {
public:
	static std::string make_suggestion(int stress_level, std::vector<float> features, std::string bearer_token_path);
};

