#include "stdafx.h"
#include "StressSolve.h"
#include "RandomForest.h"
#include "SuggestionMaker.h"
#include "Student.h"
#include "Display.h"
#include "json.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

StressSolve::StressSolve() {
	rf = new RandomForest();
	students = std::vector<Student>();
	suggestion_maker = SuggestionMaker();
	Display display = Display();
}

void StressSolve::add_student(Student student) {
	students.push_back(student);
}

void StressSolve::predict(Student student) {
	std::vector<float> features = student.get_features();
	int prediction = rf->predict(features);
	display.display_stress_level(prediction);
}

void StressSolve::give_suggestion(Student student) {
	std::vector<float> features = student.get_features();
	int prediction = rf->predict(features);
	std::string suggestion = suggestion_maker.make_suggestion(prediction, features);
	display.display_suggestion(suggestion);
}

void StressSolve::test_train_model(int tree_count, int max_depth, int fold_count) {
	std::vector<std::vector<float>> X;
	std::vector<int> y;

	for (Student student : students) {
		X.push_back(student.get_features());
		y.push_back(student.get_stress_level());
	}

	RandomForest test_rf(tree_count, max_depth);
	test_rf.k_fold_cross_validation(fold_count, X, y);
}

void StressSolve::train_model(int tree_count, int max_depth) {
	std::vector<std::vector<float>> X;
	std::vector<int> y;

	for (Student student : students) {
		X.push_back(student.get_features());
		y.push_back(student.get_stress_level());
	}

	rf = new RandomForest(tree_count, max_depth);
	rf->fit(X, y);
}


void StressSolve::load_model(std::string path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cout << "Error: Could not open file " << path << std::endl;
		return;
	}

	json j;
	file >> j;
	file.close();

	rf->from_json(j);
}

void StressSolve::save_model(std::string path) {
	std::ofstream file(path);
	if (!file.is_open()) {
		std::cout << "Error: Could not open file " << path << std::endl;
		return;
	}

	file << rf->to_json().dump(4);
	file.close();
}

