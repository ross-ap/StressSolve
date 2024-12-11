#pragma once
#include <string>
#include <vector>

class Student {
public:
    std::vector<float> features;
	int stress_level;

    Student(const std::vector<float>& features, int stress_level = -1);

	std::vector<float> get_features();
	int get_stress_level();

	void set_stress_level(int stress_level);
};

