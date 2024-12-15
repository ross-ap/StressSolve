#include "Node.h"
#include "json.hpp"

using json = nlohmann::json;

Node::Node(int feature_index, float threshold) {

	if (feature_index < 0 || feature_index > 20) {
		throw std::invalid_argument("feature_index must be between 0 and 20");
	}

	if (threshold < 0) {
		throw std::invalid_argument("threshold cannot be negative");
	}
	else if (threshold > 40) {
		throw std::invalid_argument("threshold is too large");
	}

	this->feature_index = feature_index;
	this->threshold = threshold;
	this->label = -1;
	this->is_leaf = false;
	this->left = nullptr;
	this->right = nullptr;
}

Node::Node(int label)
    : is_leaf(true), feature_index(-1), threshold(0), label(label), left(nullptr), right(nullptr) {}

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