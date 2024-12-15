#include "CppUnitTest.h"
#include "SuggestionMaker.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StressSolveTestSuggestionMaker {
	TEST_CLASS(TestSuggestionMakerClass) {

	public:
		TEST_METHOD(TestSuggestionMakerConstructor) {
			SuggestionMaker suggestion_maker;

			std::string suggestion = suggestion_maker.make_suggestion(1, { 15, 8, 1, 15, 5, 3, 1, 4, 3, 1, 2, 2, 1, 4, 1, 5, 1, 4, 5, 5 }, "bearer_token.txt");
			Assert::IsNotNull(suggestion.data());
		}

		TEST_METHOD(TestSuggestionMakerInvalidStressLevel) {
			SuggestionMaker suggestion_maker;
			try {
				std::string suggestion = suggestion_maker.make_suggestion(3, { 15, 8, 1, 15, 5, 3, 1, 4, 3, 1, 2, 2, 1, 4, 1, 5, 1, 4, 5, 5 }, "bearer_token.txt");
				Assert::Fail(L"Expected exception not thrown");
			}
			catch (std::invalid_argument e) {
				Assert::AreEqual(e.what(), "stress_level must be between 0 and 2");
			}
		}

		TEST_METHOD(TestSuggestionMakerMissingBearerToken) {
			SuggestionMaker suggestion_maker;
			std::string bearer_token_path = "missing_bearer_token.txt";
			std::string suggestion = suggestion_maker.make_suggestion(1, { 15, 8, 1, 15, 5, 3, 1, 4, 3, 1, 2, 2, 1, 4, 1, 5, 1, 4, 5, 5 }, bearer_token_path);
			std::string output = "Error: Could not open file " + bearer_token_path + " or it is empty.\nPlease make sure the file exists and contains the bearer token.";
			Assert::AreEqual(suggestion.c_str(), output.c_str());
		}

		TEST_METHOD(TestSuggestionMakerEmptyBearerToken) {
			SuggestionMaker suggestion_maker;
			std::string bearer_token_path = "empty_bearer_token.txt";
			std::string suggestion = suggestion_maker.make_suggestion(1, { 15, 8, 1, 15, 5, 3, 1, 4, 3, 1, 2, 2, 1, 4, 1, 5, 1, 4, 5, 5 }, bearer_token_path);
			std::string output = "Error: Could not open file " + bearer_token_path + " or it is empty.\nPlease make sure the file exists and contains the bearer token.";
			Assert::AreEqual(suggestion.c_str(), output.c_str());
		}
	};
}