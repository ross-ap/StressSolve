#pragma once
#include "json.hpp"
using json = nlohmann::json;

class Node {
private:
    bool is_leaf;
    int feature_index;
    float threshold;
    int label;
    Node* left;
    Node* right;

public:

    Node(int feature_index, float threshold);
    Node(int label);
    ~Node();

	Node* get_left() { return left; }
	Node* get_right() { return right; }

	void set_left(Node* left) { this->left = left; }
	void set_right(Node* right) { this->right = right; }

	bool is_leaf_node() { return is_leaf; }

	int get_label() { return label; }
	void set_label(int label) { this->label = label; }
	int get_feature_index() { return feature_index; }
	void set_feature_index(int feature_index) { this->feature_index = feature_index; }
	float get_threshold() { return threshold; }
	void set_threshold(float threshold) { this->threshold = threshold; }

    json to_json() const;
    static Node* from_json(const json& j);
};

