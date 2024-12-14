#pragma once
#include <string>
#include <vector>

#include "RandomForest.h"
#include "Student.h"
#include "SuggestionMaker.h"


class StressSolve
{
private:
	RandomForest* rf;
	std::vector<Student> students;
	SuggestionMaker suggestion_maker;

public:
	StressSolve();

	RandomForest* get_rf();

	void add_student(Student student);

	void load_model(std::string path);
	void save_model(std::string path);

	float test_train_model(int tree_count, int max_depth, int fold_count);
	void train_model(int tree_count, int max_depth);
	void update_model();

	int predict(Student student);
	std::string give_suggestion(Student student);
};

