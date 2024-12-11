#include "stdafx.h"

#include <string>
#include <vector>

#include "Student.h"

Student::Student(const std::vector<float>& features, int stress_level)
        : features(features), stress_level(stress_level) {}

std::vector<float> Student::get_features() {
	return features;
}

int Student::get_stress_level() {
	return stress_level;
}

void Student::set_stress_level(int stress_level) {
	this->stress_level = stress_level;
}
