#pragma once

#include <vector>
#include <string>

class DataHandler {
private:
	std::vector<std::vector<float>> features;
	std::vector<int> labels;

	void calculateMinMax(const std::vector<std::vector<float>>& data, std::vector<float>& minVals, std::vector<float>& maxVals);
	void normalizeDataset(std::vector<std::vector<float>>& data, const std::vector<float>& minVals, const std::vector<float>& maxVals);
	float euclideanDistance(const std::vector<float>& v1, const std::vector<float>& v2, std::vector<bool>& missing);
	void knnImpute(int k);
	void denormalizeDataset(std::vector<std::vector<float>>& data, const std::vector<float>& minVals, const std::vector<float>& maxVals);

public:
	DataHandler();

	void loadDataset(std::string path);
	void imputeMissingValues();

};

