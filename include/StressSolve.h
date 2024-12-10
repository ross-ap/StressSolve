#pragma once
#include "RandomForest.h"
#include "Student.h"
#include "SuggestionMaker.h"
#include "Display.h"

#include <string>
#include <vector>


class StressSolve
{
private:
	std::vector<Student> students;
	SuggestionMaker suggestion_maker;
	Display display;

public:
	RandomForest* rf;
	StressSolve();
	void add_student(Student student);

	int predict(Student student);

	void give_suggestion(Student student);

	float test_train_model(int tree_count, int max_depth, int fold_count);

	void train_model(int tree_count, int max_depth);

	void load_model(std::string path);

	void save_model(std::string path);
};

