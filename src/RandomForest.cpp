#include "stdafx.h"
#include "RandomForest.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <unordered_map>

RandomForest::RandomForest() {
	n_trees = 0;
	max_depth = 0;
}

RandomForest::RandomForest(int n_trees, int max_depth)
    : n_trees(n_trees), max_depth(max_depth) {
    // Initialize the trees
    for (int i = 0; i < n_trees; ++i) {
        trees.emplace_back(max_depth);
    }
}

void RandomForest::fit(const std::vector<std::vector<float>>& X, const std::vector<int>& y) {
    for (auto& tree : trees) {
        auto [bootstrapped_X, bootstrapped_y] = bootstrap(X, y);
        tree.fit(bootstrapped_X, bootstrapped_y);
    }
}

int RandomForest::predict(const std::vector<float>& x) {
    std::vector<int> predictions;
    for (auto& tree : trees) {
        predictions.push_back(tree.predict(x));
    }
    return majority_vote(predictions);
}

float RandomForest::k_fold_cross_validation(int fold_count, const std::vector<std::vector<float>>& X, const std::vector<int>& y) {
    int n = X.size();
    int fold_size = n / fold_count;
    float total_accuracy = 0.0f;

    // Shuffle the dataset to ensure random distribution
    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0); // Fill indices with 0, 1, ..., n-1
    std::shuffle(indices.begin(), indices.end(), std::mt19937{ std::random_device{}() });

    for (int i = 0; i < fold_count; ++i) {
        // Determine the indices for the validation fold
        int start = i * fold_size;
        int end = (i == fold_count - 1) ? n : start + fold_size; // Last fold might take the remainder

        // Create training and validation sets
        std::vector<std::vector<float>> train_X, test_X;
        std::vector<int> train_y, test_y;

        for (int j = 0; j < n; ++j) {
            if (j >= start && j < end) {
                // Validation data
                test_X.push_back(X[indices[j]]);
                test_y.push_back(y[indices[j]]);
            }
            else {
                // Training data
                train_X.push_back(X[indices[j]]);
                train_y.push_back(y[indices[j]]);
            }
        }

        // Fit the random forest on the training data
        this->fit(train_X, train_y);

        // Evaluate on the test data
        int correct_predictions = 0;
        for (int j = 0; j < test_X.size(); ++j) {
            if (this->predict(test_X[j]) == test_y[j]) {
                correct_predictions++;
            }
        }

        float accuracy = static_cast<float>(correct_predictions) / test_X.size();
        total_accuracy += accuracy;
        std::cout << "Fold " << (i + 1) << " Accuracy: " << accuracy << std::endl;
    }

    return total_accuracy / fold_count; // Average accuracy across folds
}

void RandomForest::print_forest_structure() {
    for (int i = 0; i < trees.size(); ++i) {
        std::cout << "Tree " << i + 1 << ":" << std::endl;
        trees[i].print_tree_structure();
        std::cout << std::endl;
    }
}

std::pair<std::vector<std::vector<float>>, std::vector<int>> RandomForest::bootstrap(const std::vector<std::vector<float>>& X, const std::vector<int>& y) {
    std::vector<std::vector<float>> bootstrapped_X;
    std::vector<int> bootstrapped_y;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, X.size() - 1);

    for (int i = 0; i < X.size(); ++i) {
        int index = distribution(generator);
        bootstrapped_X.push_back(X[index]);
        bootstrapped_y.push_back(y[index]);
    }
    return { bootstrapped_X, bootstrapped_y };
}

int RandomForest::majority_vote(const std::vector<int>& predictions) {
    std::unordered_map<int, int> count;
    for (int pred : predictions) {
        count[pred]++;
    }
    auto maxElementIt = std::max_element(count.begin(), count.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
        });

    if (maxElementIt != count.end()) {
        return maxElementIt->first;  // Safe to dereference
    }
    else {
        // Handle the case when count is empty, e.g., return a default value or throw an exception
        throw std::runtime_error("Count is empty");
    }
}

json RandomForest::to_json() const {
    json j;
    j["n_trees"] = n_trees;
    j["max_depth"] = max_depth;
    for (const auto& tree : trees) {
        j["trees"].push_back(tree.to_json());
    }
    return j;
}

void RandomForest::from_json(const json& j) {
    n_trees = j["n_trees"];
    max_depth = j["max_depth"];
    trees.clear();
    for (const auto& tree_json : j["trees"]) {
        DecisionTree tree;
        tree.from_json(tree_json);
        trees.push_back(tree);
    }
}