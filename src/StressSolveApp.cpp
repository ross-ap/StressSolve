#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "StressSolveApp.h"
#include <QFileDialog>  // Include QFileDialog


QStringList static generate_range(int start, int end) {
	QStringList range;
    for (int i = start; i <= end; i++) {
		range.append(QString::number(i));
    }
    return range;
}

StressSolveApp::StressSolveApp(QWidget *parent): QMainWindow(parent) {
    ui.setupUi(this);
    dataset = new DataHandler();
    stressSolve = new StressSolve();

	ui.comboBox->addItems(generate_range(0, 21));
	ui.comboBox_2->addItems(generate_range(0, 30));
	ui.comboBox_3->addItems(generate_range(0, 1));
	ui.comboBox_4->addItems(generate_range(0, 27));
	ui.comboBox_5->addItems(generate_range(0, 5));
	ui.comboBox_6->addItems(generate_range(1, 3));
	ui.comboBox_7->addItems(generate_range(0, 5));
	ui.comboBox_8->addItems(generate_range(0, 5));
	ui.comboBox_9->addItems(generate_range(0, 5));
	ui.comboBox_10->addItems(generate_range(0, 5));
	ui.comboBox_11->addItems(generate_range(0, 5));
	ui.comboBox_12->addItems(generate_range(0, 5));
	ui.comboBox_13->addItems(generate_range(0, 5));
	ui.comboBox_14->addItems(generate_range(0, 5));
	ui.comboBox_15->addItems(generate_range(0, 5));
	ui.comboBox_16->addItems(generate_range(0, 5));
	ui.comboBox_17->addItems(generate_range(0, 3));
	ui.comboBox_18->addItems(generate_range(0, 5));
	ui.comboBox_19->addItems(generate_range(0, 5));
	ui.comboBox_20->addItems(generate_range(0, 5));
}

StressSolveApp::~StressSolveApp() {
}

std::pair<std::string, std::string> rsplit(const std::string& str, char delimiter) {
    size_t pos = str.rfind(delimiter);
    return pos != std::string::npos ? std::make_pair(str.substr(0, pos), str.substr(pos + 1)) : std::make_pair(str, "");
}

void StressSolveApp::on_modelCreateNewButton_clicked() {

    QString fileFullPath = QFileDialog::getSaveFileName(this, "Create new Model", selectedFilePath, "JSON Files (*.json)");

    if (!fileFullPath.isEmpty()) {

        QMessageBox::information(this, "Saved", "Saved as " + fileFullPath);

        selectedFilePath = fileFullPath;
        auto [filePath, fileName] = rsplit(selectedFilePath.toStdString(), '/');

        ui.modelName->setText(QString::fromStdString(rsplit(fileName, '.').first));
        ui.modelPath->setText(QString::fromStdString(filePath));

        ui.datasetChooseButton->setEnabled(true);
    }
}

void StressSolveApp::on_modelLoadButton_clicked() {

    QString fileFullPath = QFileDialog::getOpenFileName(this, "Select Model JSON", "", "JSON Files (*.json)");

    if (!fileFullPath.isEmpty()) {
        selectedFilePath = fileFullPath;
        auto [filePath, fileName] = rsplit(selectedFilePath.toStdString(), '/');

        ui.modelName->setText(QString::fromStdString(rsplit(fileName, '.').first));
        ui.modelPath->setText(QString::fromStdString(filePath));

        stressSolve->load_model(selectedFilePath.toStdString());

        ui.treeMaxDepth->setValue(stressSolve->rf->get_n_trees());
        ui.numberOfTrees->setValue(stressSolve->rf->get_max_depth());

        ui.datasetChooseButton->setEnabled(true);
    }
}

void StressSolveApp::on_modelSaveButton_clicked() {
    stressSolve->save_model(selectedFilePath.toStdString());
    QMessageBox::information(this, "Saved", "Saved as " + selectedFilePath);
}

void StressSolveApp::on_modelTestTrainButton_clicked() {
    int percentTrainData = ui.percentTrainData->value();

	int n = dataset->get_labels().size();
	int n_train = n * percentTrainData / 100;

	std::vector<Student> students;
	for (int i = 0; i < n_train; i++) {
		Student student(dataset->get_features()[i], dataset->get_labels()[i]);
		stressSolve->add_student(student);
	}

	int n_trees = ui.numberOfTrees->value();
	int max_depth = ui.treeMaxDepth->value();
	int fold_count = ui.numberOfKFold->value();

	float accuracy_percent = stressSolve->test_train_model(n_trees, max_depth, fold_count);

	ui.kFoldAccuracy->setText(QString::number(accuracy_percent * 100) + "%");
}

void StressSolveApp::on_modelTrainButton_clicked() {

    int percentTrainData = ui.percentTrainData->value();

    int n = dataset->get_labels().size();
    int n_train = n * percentTrainData / 100;

    std::vector<Student> students;
    for (int i = 0; i < n_train; i++) {
        Student student(dataset->get_features()[i], dataset->get_labels()[i]);
        stressSolve->add_student(student);
    }

    int n_trees = ui.numberOfTrees->value();
    int max_depth = ui.treeMaxDepth->value();

	stressSolve->train_model(n_trees, max_depth);
	QMessageBox::information(this, "Model Training", "Model trained with " + QString::number(n_trees) + " trees and max depth " + QString::number(max_depth));
}

void StressSolveApp::setTrainingWidgetStates(bool state) {
    ui.treeMaxDepth->setEnabled(state);
    ui.numberOfTrees->setEnabled(state);
    ui.numberOfKFold->setEnabled(state);
    ui.percentTrainData->setEnabled(state);

    ui.modelTestTrainButton->setEnabled(state);
    ui.modelTrainButton->setEnabled(state);
}

void StressSolveApp::loadTable() {
    ui.datasetTable->clear();
    ui.datasetTable->setRowCount(0);
    ui.datasetTable->setColumnCount(0);

    std::vector<std::vector<float>> features = dataset->get_features();
    std::vector<int> labels = dataset->get_labels();

    ui.datasetTable->setColumnCount(features[0].size() + 1);

    QStringList headers;
    for (std::string header : dataset->get_feature_headers()) {
        headers.append(QString::fromStdString(header));
    }
    ui.datasetTable->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < features.size(); row++) {
        ui.datasetTable->insertRow(row);
        for (int col = 0; col < features[row].size(); col++) {
            ui.datasetTable->setItem(row, col, new QTableWidgetItem(QString::number(features[row][col])));
        }

        ui.datasetTable->setItem(row, features[row].size(), new QTableWidgetItem(QString::number(labels[row])));
    }

    ui.datasetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.datasetFeatureCount->setText("Features: " + QString::number(features[0].size() + 1));
    ui.datasetLineCount->setText("Lines: " + QString::number(labels.size()));
    ui.datasetMissingCount->setText("Number of lines with missing features: " + QString::number(dataset->get_numOfLineMissingFeatures()));

    if (dataset->get_numOfLineMissingFeatures() > 0) {
        ui.imputeKNNLimit->setEnabled(true);
        ui.imputeKNNLimit->setMaximum(std::max(10, (int)labels.size() / 50));
        ui.imputeKNNLimit->setMinimum(std::min(5, (int)labels.size() / 50));

        ui.imputeButton->setEnabled(true);

        setTrainingWidgetStates(false);
    }
    else {
        setTrainingWidgetStates(true);
    }
}

void StressSolveApp::on_datasetChooseButton_clicked() {

    QString fileFullPath = QFileDialog::getOpenFileName(this, "Select Dataset", "", "CSV Files (*.csv)");

    if (!fileFullPath.isEmpty()) {
        ui.datasetPath->setText(fileFullPath);

        dataset->loadDataset(fileFullPath.toStdString());

        loadTable();
    }
}

void StressSolveApp::on_imputeButton_clicked() {
    dataset->imputeMissingValues(ui.imputeKNNLimit->value());
    loadTable();

    ui.imputeKNNLimit->setEnabled(false);
    ui.imputeButton->setEnabled(false);
    ui.datasetMissingCount->setText("Number of lines with missing features: " + QString::number(dataset->get_numOfLineMissingFeatures()));

    setTrainingWidgetStates(true);

    QMessageBox::information(this, "Data Imputation", "Imputation finished.\nImputed with neighbour count: " + QString::number(ui.imputeKNNLimit->value()));
}

void StressSolveApp::on_predictButton_clicked() {
	std::vector<float> features;

    // Till combobox_20
	features.push_back(ui.comboBox->currentText().toFloat());
	features.push_back(ui.comboBox_2->currentText().toFloat());
	features.push_back(ui.comboBox_3->currentText().toFloat());
	features.push_back(ui.comboBox_4->currentText().toFloat());
	features.push_back(ui.comboBox_5->currentText().toFloat());
	features.push_back(ui.comboBox_6->currentText().toFloat());
	features.push_back(ui.comboBox_7->currentText().toFloat());
	features.push_back(ui.comboBox_8->currentText().toFloat());
	features.push_back(ui.comboBox_9->currentText().toFloat());
	features.push_back(ui.comboBox_10->currentText().toFloat());
	features.push_back(ui.comboBox_11->currentText().toFloat());
	features.push_back(ui.comboBox_12->currentText().toFloat());
	features.push_back(ui.comboBox_13->currentText().toFloat());
	features.push_back(ui.comboBox_14->currentText().toFloat());
	features.push_back(ui.comboBox_15->currentText().toFloat());
	features.push_back(ui.comboBox_16->currentText().toFloat());
	features.push_back(ui.comboBox_17->currentText().toFloat());
	features.push_back(ui.comboBox_18->currentText().toFloat());
	features.push_back(ui.comboBox_19->currentText().toFloat());
	features.push_back(ui.comboBox_20->currentText().toFloat());

	Student student(features);
	int prediction = stressSolve->predict(student);
	ui.predict_output->setText(QString::number(prediction));

}
