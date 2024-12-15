#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StressSolveApp.h"

#include "DataHandler.h"
#include "StressSolve.h"

class StressSolveApp : public QMainWindow {
    Q_OBJECT

private slots:
    void on_modelCreateNewButton_clicked();
    void on_modelLoadButton_clicked();
    void on_modelSaveButton_clicked();
    void on_modelTestTrainButton_clicked();
    void on_modelTrainButton_clicked();
    void on_modelFineTuneButton_clicked();

    void on_datasetChooseButton_clicked();
    void on_imputeButton_clicked();

	void on_predictModelChooseButton_clicked();
    void on_predictButton_clicked();

    void on_bearerTokenPathChooseButton_clicked();
    void on_getSuggestionButton_clicked();

private:
    Ui::StressSolveAppClass ui;
    QString selectedFilePath;
    DataHandler* dataset;
    StressSolve* stressSolve;
	std::vector<QComboBox*> featureComboBoxes;
    Student* currentStudent;

    void reset();
    void loadTable();
    void setTrainingWidgetStates(bool state);

public:
    StressSolveApp(QWidget* parent = nullptr);
    ~StressSolveApp();
};
