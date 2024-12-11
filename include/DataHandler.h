#pragma once

#include <string>
#include <vector>

class DataHandler {
private:
	int numOfLineMissingFeatures;
	std::vector<std::vector<float>> features;
	std::vector<int> labels;
	std::vector<std::string> feature_headers;

	void reset();

	float euclideanDistance(const std::vector<float>& v1, const std::vector<float>& v2, std::vector<bool>& missing);

	void calculateMinMax(const std::vector<std::vector<float>>& data, std::vector<float>& minVals, std::vector<float>& maxVals);
	void normalizeDataset(std::vector<std::vector<float>>& data, const std::vector<float>& minVals, const std::vector<float>& maxVals);
	void knnImpute(int k);
	void denormalizeDataset(std::vector<std::vector<float>>& data, const std::vector<float>& minVals, const std::vector<float>& maxVals);

public:
	DataHandler();

	int get_numOfLineMissingFeatures();
	std::vector<std::vector<float>> get_features();
	std::vector<int> get_labels();
	std::vector<std::string> get_feature_headers();

	void loadDataset(std::string path);
	void imputeMissingValues(int neighbour_count);
};

