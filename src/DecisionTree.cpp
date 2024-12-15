#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "DecisionTree.h"
#include "Node.h"


Node* DecisionTree::build_tree(const std::vector<std::vector<float>>& features, const std::vector<int>& labels, int depth) {
    if (depth >= max_depth && max_depth != 0) return create_leaf(labels);
    if (is_pure(labels)) return create_leaf(labels);

    int best_feature;
    float best_threshold;
    if (!find_best_split(features, labels, best_feature, best_threshold)) {
        return create_leaf(labels);
    }

    auto [left_features, left_labels, right_features, right_labels] = split_data(features, labels, best_feature, best_threshold);

    Node* node = new Node(best_feature, best_threshold);

    node->set_left(build_tree(left_features, left_labels, depth + 1));
    node->set_right(build_tree(right_features, right_labels, depth + 1));
    return node;
}

void DecisionTree::update_tree(Node* node, const std::vector<std::vector<float>>& features, const std::vector<int>& labels) {
    if (!node) return;

    if (node->is_leaf_node()) {
        int updated_label = create_leaf(labels)->get_label();
        node->set_label(updated_label);
        return;
    }

    int best_feature;
    float best_threshold;
    if (find_best_split(features, labels, best_feature, best_threshold)) {
        node->set_feature_index(best_feature);
        node->set_threshold(best_threshold);

        auto [left_features, left_labels, right_features, right_labels] = split_data(features, labels, best_feature, best_threshold);

        if (!left_labels.empty() && !right_labels.empty()) {
            if (!node->get_left()) node->set_left(new Node(-1));
            if (!node->get_right()) node->set_right(new Node(-1));

            update_tree(node->get_left(), left_features, left_labels);
            update_tree(node->get_right(), right_features, right_labels);
        }
    }
}

Node* DecisionTree::create_leaf(const std::vector<int>& labels) {
    std::map<int, int> counts;
    for (int label : labels) counts[label]++;
    int majority_label = std::max_element(counts.begin(), counts.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; })->first;
    return new Node(majority_label);
}

bool DecisionTree::is_pure(const std::vector<int>& labels) {
    return std::all_of(labels.begin(), labels.end(), [&](int label) { return label == labels[0]; });
}

bool DecisionTree::find_best_split(const std::vector<std::vector<float>>& features, const std::vector<int>& labels,
    int& best_feature, float& best_threshold) {
    float best_impurity = std::numeric_limits<float>::max();
    bool found_split = false;

    for (int feature_index = 0; feature_index < features[0].size(); ++feature_index) {
        std::vector<std::pair<float, int>> feature_labels(features.size());
        for (int i = 0; i < features.size(); ++i) feature_labels[i] = { features[i][feature_index], labels[i] };
        std::sort(feature_labels.begin(), feature_labels.end());

        for (int i = 1; i < feature_labels.size(); ++i) {
            if (feature_labels[i].first == feature_labels[i - 1].first) continue;

            float threshold = (feature_labels[i].first + feature_labels[i - 1].first) / 2.0;

            std::vector<int> left_labels, right_labels;
            for (const auto& [feature_value, label] : feature_labels) {
                if (feature_value < threshold) left_labels.push_back(label);
                else right_labels.push_back(label);
            }

            float impurity = calculate_gini(left_labels) * left_labels.size() / labels.size() +
                calculate_gini(right_labels) * right_labels.size() / labels.size();

            if (impurity < best_impurity) {
                best_impurity = impurity;
                best_feature = feature_index;
                best_threshold = threshold;
                found_split = true;
            }
        }
    }
    return found_split;
}

std::tuple<std::vector<std::vector<float>>, std::vector<int>, std::vector<std::vector<float>>, std::vector<int>>
DecisionTree::split_data(const std::vector<std::vector<float>>& features, const std::vector<int>& labels,
    int feature_index, float threshold) {
    std::vector<std::vector<float>> left_features, right_features;
    std::vector<int> left_labels, right_labels;
    for (int i = 0; i < features.size(); ++i) {
        if (features[i][feature_index] < threshold) {
            left_features.push_back(features[i]);
            left_labels.push_back(labels[i]);
        }
        else {
            right_features.push_back(features[i]);
            right_labels.push_back(labels[i]);
        }
    }
    return { left_features, left_labels, right_features, right_labels };
}

float DecisionTree::calculate_gini(const std::vector<int>& labels) {
    std::map<int, int> label_counts;
    for (int label : labels) label_counts[label]++;
    float impurity = 1.0;
    for (const auto& [label, count] : label_counts) {
        float prob = static_cast<float>(count) / labels.size();
        impurity -= prob * prob;
    }
    return impurity;
}

int DecisionTree::traverse_tree(Node* node, const std::vector<float>& features) {
    if (!node) throw std::runtime_error("Tree structure error: Node is nullptr in traverse_tree.");
    if (node->is_leaf_node()) return node->get_label();
    if (features[node->get_feature_index()] < node->get_threshold()) return traverse_tree(node->get_left(), features);
    return traverse_tree(node->get_right(), features);
}

DecisionTree::DecisionTree(int max_depth)
    : root(nullptr), max_depth(max_depth) {}

void DecisionTree::fit(const std::vector<std::vector<float>>& features, const std::vector<int>& labels) {
    root = build_tree(features, labels, 0);
}

void DecisionTree::update(const std::vector<std::vector<float>>& features, const std::vector<int>& labels) {
    update_tree(root, features, labels);
}

int DecisionTree::predict(const std::vector<float>& features) {
    return traverse_tree(root, features);
}

json DecisionTree::to_json() const {
    json j;
    j["root"] = root ? root->to_json() : nullptr;
    return j;
}

void DecisionTree::from_json(const json& j) {
    if (j.contains("root")) {
        root = Node::from_json(j["root"]);
    }
}