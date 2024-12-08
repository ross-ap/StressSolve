#include "stdafx.h"
#include "DataHandler.h"
#include <limits>
#include <fstream>
#include <algorithm>
#include <sstream>

DataHandler::DataHandler() {
}

void DataHandler::loadDataset(std::string path) {
	std::ifstream in(path);
	std::string line;

	std::getline(in, line);
	std::istringstream ss(line);
	std::string token;
	int numOfFeatures = 0;
	while (std::getline(ss, token, ',')) {
		numOfFeatures++;
	}
	
	while (std::getline(in, line)) {
		std::vector<float> feature;
		std::string token;
		std::istringstream ss(line);
		while (std::getline(ss, token, ',')) {
			if (token == "") {
				feature.push_back(NAN);
			}
			else {
				feature.push_back(std::stof(token));
			}
		}

		if (feature.size() != numOfFeatures) continue;

		labels.push_back(feature.back());
		feature.pop_back();
		features.push_back(feature);
	}

	imputeMissingValues();
}

void DataHandler::imputeMissingValues() {
	std::vector<float> minVals;
	std::vector<float> maxVals;

	calculateMinMax(features, minVals, maxVals);
	normalizeDataset(features, minVals, maxVals);
	knnImpute(std::max(10, (int)features.size() / 50));
	denormalizeDataset(features, minVals, maxVals);

}

void DataHandler::calculateMinMax(const std::vector<std::vector<float>>& data, std::vector<float>& minVals, std::vector<float>& maxVals) {

	int numOfFeatures = data[0].size();

	minVals.assign(numOfFeatures, std::numeric_limits<double>::max());
	maxVals.assign(numOfFeatures, std::numeric_limits<double>::lowest());

	for (int i = 1; i < data.size(); i++) {
		for (int j = 0; j < data[i].size(); j++) {
			if (isnan(data[i][j])) continue;

			minVals[j] = std::min(minVals[j], data[i][j]);
			maxVals[j] = std::max(maxVals[j], data[i][j]);
		}
	}
}

void DataHandler::normalizeDataset(std::vector<std::vector<float>>& data, const std::vector<float>& minVals, const std::vector<float>& maxVals) {
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < data[i].size(); j++) {
			if (isnan(data[i][j])) continue;

			data[i][j] = (data[i][j] - minVals[j]) / (maxVals[j] - minVals[j]);
		}
	}
}

float DataHandler::euclideanDistance(const std::vector<float>& v1, const std::vector<float>& v2, std::vector<bool>& missing) {
	float sum = 0.0;
	for (int i = 0; i < v1.size(); i++) {
		if (missing[i]) continue;
		sum += pow(v1[i] - v2[i], 2);
	}
	return sqrt(sum);
}

void DataHandler::knnImpute(int k) {

	for (int i = 0; i < features.size(); i++) {
		for (int j = 0; j < features[i].size(); j++) {
			if (isnan(features[i][j])) {

				std::vector<bool> missing(features[i].size(), false);
				missing[j] = true;

				std::vector<std::pair<float, int>> distances;
				for (int m = 0; m < features.size(); m++) {
					if (i == m || isnan(features[m][j])) continue;
					float dist = euclideanDistance(features[i], features[m], missing);
					distances.push_back({ dist, m });
				}

				std::sort(distances.begin(), distances.end());

				int count = 0;
				float sum = 0.0;
				for (int m = 0; m < k && m < distances.size(); m++) {
					sum += features[distances[m].second][j];
					count++;
				}

				if (count == 0) continue;
				features[i][j] = sum / k;

			}
		}
	}
}

void DataHandler::denormalizeDataset(std::vector<std::vector<float>>& data, const std::vector<float>& minVals, const std::vector<float>& maxVals) {
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < data[i].size(); j++) {
			data[i][j] = data[i][j] * (maxVals[j] - minVals[j]) + minVals[j];
		}
	}
}
