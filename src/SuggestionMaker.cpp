#include "SuggestionMaker.h"

#include <fstream>
#include <string>
#include <vector>

#include <curl/curl.h>
#include <json.hpp>

#include <QtWidgets>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string SuggestionMaker::make_suggestion(int stress_level, std::vector<float> features) {
    std::string suggestionMessage = "Something went wrong. Please try again.";

	std::ifstream file("bearer_token.txt");
	if (!file.is_open()) {
		return "Error: Could not open file bearer_token.txt.\nPlease make sure the file exists and contains the bearer token.";
	}

	std::string bearer_token;
	file >> bearer_token;
	file.close();

	qDebug() << "Bearer token: " << QString::fromStdString(bearer_token);

	std::vector<std::string> featureNames = {
		"anxiety_level","self_esteem","mental_health_history","depression",
		"headache","blood_pressure","sleep_quality","breathing_problem",
		"noise_level","living_conditions","safety","basic_needs",
		"academic_performance","study_load","teacher_student_relationship","future_career_concerns",
		"social_support","peer_pressure","extracurricular_activities","bullying"
	};

    CURL* curl = curl_easy_init();

    if (curl) {

		std::string prompt = R"(You are an assistant trained to give advice to students having potential stress issue.
			The following are the features of the student (the higher number means the higher value of the feature):)";

		for (int i = 0; i < features.size(); i++) {
			prompt += "\n" + featureNames[i] + ": " + std::to_string(features[i]);
		}

		if (stress_level == 0) {
			prompt += "\nThe student is likely not stressed.";
		}
		else if (stress_level == 1) {
			prompt += "\nThe student is likely stressed.";
		}
		else {
			prompt += "\nThe student is likely highly stressed.";
		}

		prompt += "\nPlease provide a suggestion to help the student.";

		nlohmann::json json_data = {
			{"model", "grok-beta"},
            {"prompt", prompt},
			{"max_tokens", 1000}
		};
		std::string json_str = json_data.dump();
        std::string url = "https://api.x.ai/v1/completions";
        std::string response_data;

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + bearer_token).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        CURLcode res = curl_easy_perform(curl);
		try {
			nlohmann::json json_response = nlohmann::json::parse(response_data);
			suggestionMessage = json_response["choices"][0]["text"];
		}
		catch (nlohmann::json::exception& e) {
			return suggestionMessage;
		}

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

	if (suggestionMessage == "<|eos|>") {
		suggestionMessage = "Something went wrong. Please try again.";
	}

    return suggestionMessage;
}
