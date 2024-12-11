#pragma once
#include <tuple>
#include <vector>

#include "Node.h"

class DecisionTree {
private:
    Node* root;
    int max_depth;

    Node* build_tree(const std::vector<std::vector<float>>& features, const std::vector<int>& labels, int depth);
    Node* create_leaf(const std::vector<int>& labels);

    bool is_pure(const std::vector<int>& labels);
    bool find_best_split(const std::vector<std::vector<float>>& features, const std::vector<int>& labels,
        int& best_feature, float& best_threshold);

    std::tuple<std::vector<std::vector<float>>, std::vector<int>, std::vector<std::vector<float>>, std::vector<int>>
        split_data(const std::vector<std::vector<float>>& features, const std::vector<int>& labels,
            int feature_index, float threshold);
    float calculate_gini(const std::vector<int>& labels);
    int traverse_tree(Node* node, const std::vector<float>& features);

public:

    DecisionTree(int max_depth = -1);

    void fit(const std::vector<std::vector<float>>& features, const std::vector<int>& labels);
    int predict(const std::vector<float>& features);

    json to_json() const;
    void from_json(const json& j);

};