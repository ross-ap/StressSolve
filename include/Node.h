#pragma once
#include "json.hpp"
using json = nlohmann::json;

class Node {
public:
    bool is_leaf;      // Decide if the node is a leaf or not
    int feature_index; // Index of the feature to split on (valid only if not a leaf)
    float threshold;   // Threshold value for the split (valid only if not a leaf)
    int label;         // Class label (valid only if a leaf)
    Node* left;        // Pointer to left child
    Node* right;       // Pointer to right child

    Node(int feature_index, float threshold);

    Node(int label);

    ~Node();

    json to_json() const;

    static Node* from_json(const json& j);
};

