#include "CppUnitTest.h"
#include "DecisionTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StressSolveTestDecisionTree {
	TEST_CLASS(TestDecisionTreeClass) {

	public:
		TEST_METHOD(TestPredict) {
			std::vector<std::vector<float>> train_features = {
				{14,20,0,11,2,1,2,4,2,3,3,2,3,2,3,3,2,3,3,2},
				{15,8,1,15,5,3,1,4,3,1,2,2,1,4,1,5,1,4,5,5},
				{12,18,1,14,2,1,2,2,2,2,3,2,2,3,3,2,2,3,2,2},
				{16,12,1,15,4,3,1,3,4,2,2,2,2,4,1,4,1,4,4,5},
				{16,28,0,7,2,3,5,1,3,2,4,3,4,3,1,2,1,5,0,5},
				{4,26,0,6,1,2,4,1,1,4,4,4,5,1,4,1,3,2,2,1},
				{5,28,0,8,1,2,4,2,2,3,5,5,5,2,4,1,3,1,1,1},
				{6,28,0,1,1,2,4,2,1,4,5,4,5,1,5,1,3,2,2,1}
			};

			std::vector<int> train_labels = { 1, 2, 1, 2, 1, 0, 0, 0 };

			DecisionTree decision_tree;
			decision_tree.fit(train_features, train_labels);

			std::vector<float> features = { 6,26,0,8,1,2,5,2,2,4,5,4,4,1,4,1,3,2,1,1 };
			int prediction = decision_tree.predict(features);
			Assert::AreEqual(prediction, 0);
		}
	};
}
