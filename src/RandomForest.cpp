#include "stdafx.h"

#include <iostream>
#include <numeric>
#include <random>
#include <unordered_map>
#include <vector>

#include "RandomForest.h"


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

RandomForest::RandomForest() {
	n_trees = 0;
	max_depth = 0;
}

RandomForest::RandomForest(int n_trees, int max_depth)
    : n_trees(n_trees), max_depth(max_depth) {
    for (int i = 0; i < n_trees; ++i) {
        trees.emplace_back(max_depth);
    }
}

int RandomForest::get_n_trees() {
    return n_trees;
}

int RandomForest::get_max_depth() {
    return max_depth;
}

void RandomForest::fit(const std::vector<std::vector<float>>& features, const std::vector<int>& labels) {
    for (auto& tree : trees) {
        auto [bootstrapped_X, bootstrapped_y] = bootstrap(features, labels);
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

float RandomForest::k_fold_cross_validation(int fold_count, const std::vector<std::vector<float>>& features, const std::vector<int>& labels) {
    int n = features.size();
    int fold_size = n / fold_count;
    float total_accuracy = 0.0f;

    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), std::mt19937{ std::random_device{}() });

    for (int i = 0; i < fold_count; ++i) {
        int start = i * fold_size;
        int end = (i == fold_count - 1) ? n : start + fold_size;

        std::vector<std::vector<float>> train_features, test_features;
        std::vector<int> train_labels, test_labels;

        for (int j = 0; j < n; ++j) {
            if (j >= start && j < end) {
                test_features.push_back(features[indices[j]]);
                test_labels.push_back(labels[indices[j]]);
            }
            else {
                train_features.push_back(features[indices[j]]);
                train_labels.push_back(labels[indices[j]]);
            }
        }

        this->fit(train_features, train_labels);

        int correct_predictions = 0;
        for (int j = 0; j < test_features.size(); ++j) {
            if (this->predict(test_features[j]) == test_labels[j]) {
                correct_predictions++;
            }
        }

        float accuracy = static_cast<float>(correct_predictions) / test_features.size();
        total_accuracy += accuracy;
        qDebug() << "Fold " << (i + 1) << " Accuracy: " << accuracy;
    }

    return total_accuracy / fold_count;
}

json RandomForest::to_json() const {
    json j;
    j["n_trees"] = n_trees;
    j["max_depth"] = max_depth;
	j["trees"] = json::array();
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