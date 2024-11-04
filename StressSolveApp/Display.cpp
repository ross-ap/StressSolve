#include "Display.h"
#include "Student.h"

#include <iostream>
#include <sstream>

Student Display::get_student_info() {
	std::string name;
	int age;
	std::vector<float> features;

	std::cout << "Enter student name: ";
	std::cin >> name;

	std::cout << "Enter student age: ";
	std::cin >> age;

	std::cout << "Enter student features (comma-separated): ";
	std::string features_str;
	std::cin >> features_str;

	std::stringstream ss(features_str);
	std::string feature;
	while (std::getline(ss, feature, ',')) {
		features.push_back(std::stod(feature));
	}

	return Student(name, age, features);
}

void Display::display_stress_level(int stress_level) {
	std::cout << "Predicted stress level: " << stress_level << std::endl;
}

void Display::display_suggestion(const std::string& suggestion) {
	std::cout << "Suggestion: " << suggestion << std::endl;
}
