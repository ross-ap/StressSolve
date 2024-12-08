#include "stdafx.h"
#include "Student.h"
#include <string>
#include <vector>

Student::Student(const std::string& name, int age, const std::vector<float>& features, int stress_level)
        : name(name), age(age), features(features), stress_level(stress_level) {}

std::vector<float> Student::get_features() {
	return features;
}

int Student::get_stress_level() {
	return stress_level;
}
