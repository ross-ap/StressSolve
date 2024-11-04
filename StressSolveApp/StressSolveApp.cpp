#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <map>
#include <numeric>

class Node {
public:
    bool is_leaf;      // Decide if the node is a leaf or not
    int feature_index; // Index of the feature to split on (valid only if not a leaf)
    float threshold;   // Threshold value for the split (valid only if not a leaf)
    int label;         // Class label (valid only if a leaf)
    Node* left;        // Pointer to left child
    Node* right;       // Pointer to right child

    // Constructor for internal nodes
    Node(int feature_index, float threshold)
        : feature_index(feature_index), threshold(threshold), label(-1), is_leaf(false), left(nullptr), right(nullptr) {}

    // Constructor for leaf nodes
    Node(int label)
        : is_leaf(true), feature_index(-1), threshold(0), label(label), left(nullptr), right(nullptr) {}

    // Destructor to clean up child nodes
    ~Node() {
        delete left;
        delete right;
    }
};


class DecisionTree {
public:
    Node* root;
    int max_depth;
    int min_samples_split;

    DecisionTree(int max_depth = -1, int min_samples_split = 2)
        : root(nullptr), max_depth(max_depth), min_samples_split(min_samples_split) {}

    void fit(const std::vector<std::vector<float>>& features, const std::vector<int>& labels) {
        root = build_tree(features, labels, 0);
    }

    int predict(const std::vector<float>& features) {
        return traverse_tree(root, features);
    }

    void print_tree_structure() {
        print_node(root, 0);
    }

private:
    Node* build_tree(const std::vector<std::vector<float>>& features, const std::vector<int>& labels, int depth) {
        if (depth >= max_depth && max_depth != -1) return create_leaf(labels); // Max depth reached
        if (labels.size() < min_samples_split || is_pure(labels)) return create_leaf(labels); // Stopping criteria

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

    Node* create_leaf(const std::vector<int>& labels) {
        std::map<int, int> counts;
        for (int label : labels) counts[label]++;
        int majority_label = std::max_element(counts.begin(), counts.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; })->first;
        return new Node(majority_label);
    }

    bool is_pure(const std::vector<int>& labels) {
        return std::all_of(labels.begin(), labels.end(), [&](int label) { return label == labels[0]; });
    }

    bool find_best_split(const std::vector<std::vector<float>>& features, const std::vector<int>& labels,
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
        split_data(const std::vector<std::vector<float>>& features, const std::vector<int>& labels,
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

    float calculate_gini(const std::vector<int>& labels) {
        std::map<int, int> label_counts;
        for (int label : labels) label_counts[label]++;
        float impurity = 1.0;
        for (const auto& [label, count] : label_counts) {
            float prob = static_cast<float>(count) / labels.size();
            impurity -= prob * prob;
        }
        return impurity;
    }

    int traverse_tree(Node* node, const std::vector<float>& features) {
        if (!node) throw std::runtime_error("Tree structure error: Node is nullptr in traverse_tree.");
        if (node->is_leaf) return node->label;
        if (features[node->feature_index] < node->threshold) return traverse_tree(node->left, features);
        return traverse_tree(node->right, features);
    }

    void print_node(Node* node, int depth) {
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
};

class RandomForest {
public:
    std::vector<DecisionTree> trees;
    int n_trees;
    int max_depth;
    int min_samples_split;

    RandomForest(int n_trees, int max_depth = -1, int min_samples_split = 2)
        : n_trees(n_trees), max_depth(max_depth), min_samples_split(min_samples_split) {
        // Initialize the trees
        for (int i = 0; i < n_trees; ++i) {
            trees.emplace_back(max_depth, min_samples_split);
        }
    }

    void fit(const std::vector<std::vector<float>>& X, const std::vector<int>& y) {
        for (auto& tree : trees) {
            auto [bootstrapped_X, bootstrapped_y] = bootstrap(X, y);
            tree.fit(bootstrapped_X, bootstrapped_y);
        }
    }

    int predict(const std::vector<float>& x) {
        std::vector<int> predictions;
        for (auto& tree : trees) {
            predictions.push_back(tree.predict(x));
        }
        return majority_vote(predictions);
    }

    float k_fold_cross_validation(int fold_count, const std::vector<std::vector<float>>& X, const std::vector<int>& y) {
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

    void print_forest_structure() {
        for (int i = 0; i < trees.size(); ++i) {
            std::cout << "Tree " << i + 1 << ":" << std::endl;
            trees[i].print_tree_structure();
            std::cout << std::endl;
        }
    }

private:
    std::pair<std::vector<std::vector<float>>, std::vector<int>> bootstrap(const std::vector<std::vector<float>>& X, const std::vector<int>& y) {
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

    int majority_vote(const std::vector<int>& predictions) {
        std::unordered_map<int, int> count;
        for (int pred : predictions) {
            count[pred]++;
        }
        return std::max_element(count.begin(), count.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; })->first;
    }
};

int main() {
    // Example dataset
    std::vector<std::vector<float>> X = {
       {2.5, 2.4}, {0.5, 0.7}, {1.3, 1.5}, {1.0, 1.0}, {2.0, 1.5},
       {3.0, 3.0}, {1.5, 1.7}, {2.1, 2.6}, {0.7, 0.8}, {1.8, 1.6},
       {3.3, 3.2}, {2.8, 2.7}, {1.9, 2.3}, {1.4, 1.4}, {0.9, 0.6},
       {1.7, 1.8}, {2.4, 2.0}, {3.1, 3.1}, {2.0, 2.1}, {0.6, 0.9}
    };
    std::vector<int> y = { 0, 1, 1, 0, 0, 1, 1, 1, 0, 0,
                           1, 1, 1, 0, 0, 1, 0, 1, 0, 1 }; // More labels with variety

    // Initialize the RandomForest with 10 trees, and set depth and minimum samples split
	// Loop 10 times and then create a random forest with randomized tree count between 20 - 30 and max depth between 5 - 10
	// Then do a k-fold cross validation with 5 folds
	for (int i = 0; i < 10; i++) {
		int tree_count = rand() % 10 + 20;
		int max_depth = rand() % 5 + 5;
		// Print the current iteration with the tree count and max depth
		std::cout << "Iteration " << i + 1 << " with tree count: " << tree_count << " and max depth: " << max_depth << std::endl;
		RandomForest rf(tree_count, max_depth, 2);
		rf.k_fold_cross_validation(5, X, y);
		std::cout << std::endl;
	}

	RandomForest rf(28, 8, 2);
	rf.k_fold_cross_validation(5, X, y);

    // Train the random forest on the dataset
    rf.fit(X, y);

    rf.print_forest_structure();

    // Define a new data point for prediction
    std::vector<float> new_data = { 0.55, 0.7 };

    // Predict the class for the new data point
    int prediction = rf.predict(new_data);

    // Output the prediction
    std::cout << "Predicted class: " << prediction << std::endl;

    return 0;
}