#include "CppUnitTest.h"
#include "Node.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StressSolveTestNode {
	TEST_CLASS(TestNodeClass) {

	public:
		TEST_METHOD(TestValidNodeConstructor) {
			Node node(0, 10);
			Assert::AreEqual(node.get_feature_index(), 0);
			Assert::AreEqual(node.get_threshold(), 10.0f);
			Assert::IsTrue(node.get_feature_index() >= 0);
			Assert::IsTrue(node.get_feature_index() <= 20);
		}

		TEST_METHOD(TestNodeConstructorNegativeFeatureIndex) {
			try {
				Node node(-1, 10);
				Assert::Fail(L"Expected exception not thrown");
			}
			catch (std::invalid_argument e) {
				Assert::AreEqual(e.what(), "feature_index must be between 0 and 20");
			}
		}

		TEST_METHOD(TestNodeConstructorFeatureIndexGreaterThan20) {
			try {
				Node node(100, 10);
				Assert::Fail(L"Expected exception not thrown");
			}
			catch (std::invalid_argument e) {
				Assert::AreEqual(e.what(), "feature_index must be between 0 and 20");
			}
		}

		TEST_METHOD(TestNodeConstructorNegativeThreshold) {
			try {
				Node node(0, -9);
				Assert::Fail(L"Expected exception not thrown");
			}
			catch (std::invalid_argument e) {
				Assert::AreEqual(e.what(), "threshold cannot be negative");
			}
		}

		TEST_METHOD(TestNodeConstructorThresholdGreaterThan40) {
			try {
				Node node(0, 100);
				Assert::Fail(L"Expected exception not thrown");
			}
			catch (std::invalid_argument e) {
				Assert::AreEqual(e.what(), "threshold is too large");
			}
		}
	};
}
