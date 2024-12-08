#pragma once

#include "Student.h"

class Display
{
public:
	Student get_student_info();

	void display_stress_level(int stress_level);

	void display_suggestion(const std::string& suggestion);
};

