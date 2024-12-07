#include <iostream>
#include <vector>
#include <fstream>
#include "StressSolve.h"


int main() {
     //Example dataset
    std::vector<std::vector<float>> X = {
       {2.5, 2.4}, {0.5, 0.7}, {1.3, 1.5}, {1.0, 1.0}, {2.0, 1.5},
       {3.0, 3.0}, {1.5, 1.7}, {2.1, 2.6}, {0.7, 0.8}, {1.8, 1.6},
       {3.3, 3.2}, {2.8, 2.7}, {1.9, 2.3}, {1.4, 1.4}, {0.9, 0.6},
       {1.7, 1.8}, {2.4, 2.0}, {3.1, 3.1}, {2.0, 2.1}, {0.6, 0.9}
    };
    std::vector<int> y = { 0, 1, 1, 0, 0, 1, 1, 1, 0, 0,
                           1, 1, 1, 0, 0, 1, 0, 1, 0, 1 };

	StressSolve stress_solve;
	// For each dataset above, push the student into the stress_solve object with random name and age
	for (int i = 0; i < X.size(); i++) {
		std::string name = "Student" + std::to_string(i + 1);
		int age = rand() % 5 + 15;
		Student student(name, age, X[i], y[i]);
		stress_solve.add_student(student);
	}

	for (int i = 0; i < 10; i++) {
		int tree_count = rand() % 10 + 20;
		int max_depth = rand() % 5 + 5;
		// Print the current iteration with the tree count and max depth
		std::cout << "Iteration " << i + 1 << " with tree count: " << tree_count << " and max depth: " << max_depth << std::endl;
		stress_solve.test_train_model(tree_count, max_depth, 5);
	}

	stress_solve.train_model(28, 5);
	stress_solve.save_model("random_forest.json");

	stress_solve.rf->print_forest_structure();

    // Define a new data point for prediction
    std::vector<float> new_data = { 0.55, 0.7 };

    // Predict the class for the new data point
	Student new_student("Student abc", 20, new_data);
	stress_solve.predict(new_student);
	stress_solve.give_suggestion(new_student);


    return 0;
}