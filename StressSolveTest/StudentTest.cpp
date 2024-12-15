#include "CppUnitTest.h"
#include "Student.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StressSolveTestStudent {
	TEST_CLASS(TestStudentClass) {

	public:
		TEST_METHOD(TestStudentConstructor) {
			std::vector<float> features = { 15, 8, 1, 15, 5, 3, 1, 4, 3, 1, 2, 2, 1, 4, 1, 5, 1, 4, 5, 5 };
			Student student(features, 0);

			Assert::AreEqual(student.get_stress_level(), 0);
			Assert::IsNotNull(student.get_features().data());
			Assert::AreEqual(student.get_features().size(), features.size());

			for (int i = 0; i < features.size(); i++) {
				Assert::AreEqual(student.get_features()[i], features[i]);
			}
		}
	};
}
