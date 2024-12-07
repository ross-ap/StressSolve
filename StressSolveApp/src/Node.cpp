#include "Node.h"
#include "json.hpp"

using json = nlohmann::json;

// Constructor for internal nodes
Node::Node(int feature_index, float threshold)
    : feature_index(feature_index), threshold(threshold), label(-1), is_leaf(false), left(nullptr), right(nullptr) {}

// Constructor for leaf nodes
Node::Node(int label)
    : is_leaf(true), feature_index(-1), threshold(0), label(label), left(nullptr), right(nullptr) {}

// Destructor to clean up child nodes
Node::~Node() {
    delete left;
    delete right;
}

json Node::to_json() const {
    json j;
    j["is_leaf"] = is_leaf;
    if (is_leaf) {
        j["label"] = label;
    }
    else {
        j["feature_index"] = feature_index;
        j["threshold"] = threshold;
        j["left"] = left ? left->to_json() : nullptr;
        j["right"] = right ? right->to_json() : nullptr;
    }
    return j;
}

Node* Node::from_json(const json& j) {
    Node* node;
    if (j["is_leaf"]) {
        node = new Node(j["label"]);
    }
    else {
        node = new Node(j["feature_index"], j["threshold"]);
        node->left = from_json(j["left"]);
        node->right = from_json(j["right"]);
    }
    return node;
}