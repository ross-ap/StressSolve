#pragma once
#include <string>
#include <vector>

class Student {
public:
    std::string name;
    int age;
    std::vector<float> features; // e.g., academic performance, sleep hours, etc.
	int stress_level; // 0 - low, 1 - medium, 2 - high

    Student(const std::string& name, int age, const std::vector<float>& features, int stress_level = -1);

	std::vector<float> get_features();

	int get_stress_level();
};

