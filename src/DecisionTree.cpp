#include "stdafx.h"
#include "DecisionTree.h"
#include "Node.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <stdexcept>

DecisionTree::DecisionTree(int max_depth)
    : root(nullptr), max_depth(max_depth) {}

void DecisionTree::fit(const std::vector<std::vector<float>>& features, const std::vector<int>& labels) {
    root = build_tree(features, labels, 0);
}

int DecisionTree::predict(const std::vector<float>& features) {
    return traverse_tree(root, features);
}

void DecisionTree::print_tree_structure() {
    print_node(root, 0);
}

Node* DecisionTree::build_tree(const std::vector<std::vector<float>>& features, const std::vector<int>& labels, int depth) {
    if (depth >= max_depth && max_depth != -1) return create_leaf(labels); // Max depth reached
    if (is_pure(labels)) return create_leaf(labels); // Stopping criteria

    int best_feature;
    float best_threshold;
    if (!find_best_split(features, labels, best_feature, best_threshold)) {
        return create_leaf(labels); // No valid split found
    }

    // Split data
    auto [left_features, left_labels, right_features, right_labels] = split_data(features, labels, best_feature, best_threshold);

    // Create internal node and recursively build children
    Node* node = new Node(best_feature, best_threshold);
    node->left = build_tree(left_features, left_labels, depth + 1);
    node->right = build_tree(right_features, right_labels, depth + 1);
    return node;
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
        // Sort the feature values to get possible split thresholds
        std::vector<std::pair<float, int>> feature_labels(features.size());
        for (int i = 0; i < features.size(); ++i) feature_labels[i] = { features[i][feature_index], labels[i] };
        std::sort(feature_labels.begin(), feature_labels.end());

        for (int i = 1; i < feature_labels.size(); ++i) {
            // Only consider unique split points
            if (feature_labels[i].first == feature_labels[i - 1].first) continue;

            // Calculate the threshold
            float threshold = (feature_labels[i].first + feature_labels[i - 1].first) / 2.0;

            // Split labels by threshold
            std::vector<int> left_labels, right_labels;
            for (const auto& [feature_value, label] : feature_labels) {
                if (feature_value < threshold) left_labels.push_back(label);
                else right_labels.push_back(label);
            }

            // Calculate impurity for the split
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
    if (node->is_leaf) return node->label;
    if (features[node->feature_index] < node->threshold) return traverse_tree(node->left, features);
    return traverse_tree(node->right, features);
}

void DecisionTree::print_node(Node* node, int depth) {
    if (!node) return;

    // Print indentation for the current depth level
    for (int i = 0; i < depth; ++i) std::cout << "  ";

    if (node->is_leaf) {
        std::cout << "Leaf node: Class = " << node->label << std::endl;
    }
    else {
        std::cout << "Node: Feature " << node->feature_index
            << " < " << node->threshold
            << " (Depth " << depth << ")" << std::endl;

        print_node(node->left, depth + 1);
        print_node(node->right, depth + 1);
    }
}


json DecisionTree::to_json() const {
    json j;
    j["root"] = root ? root->to_json() : nullptr;
    return j;
}

// Deserialize from JSON
void DecisionTree::from_json(const json& j) {
    if (j.contains("root")) {
        root = Node::from_json(j["root"]);
    }
}