#pragma once
#include "DecisionTree.h"

#include <vector>


class RandomForest {
public:
    std::vector<DecisionTree> trees;
    int n_trees;
    int max_depth;

    RandomForest();

    RandomForest(int n_trees, int max_depth = -1);

    void fit(const std::vector<std::vector<float>>& X, const std::vector<int>& y);

    int predict(const std::vector<float>& x);

    float k_fold_cross_validation(int fold_count, const std::vector<std::vector<float>>& X, const std::vector<int>& y);

    void print_forest_structure();

    json to_json() const;

    void from_json(const json& j);

private:
    std::pair<std::vector<std::vector<float>>, std::vector<int>> bootstrap(const std::vector<std::vector<float>>& X, const std::vector<int>& y);

    int majority_vote(const std::vector<int>& predictions);
};