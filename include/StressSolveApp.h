#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StressSolveApp.h"
#include "DataHandler.h"
#include "StressSolve.h"

class StressSolveApp : public QMainWindow
{
    Q_OBJECT

public:
    StressSolveApp(QWidget *parent = nullptr);
    ~StressSolveApp();

private slots:
    void on_modelCreateNewButton_clicked();
    void on_modelLoadButton_clicked();
    void on_modelSaveButton_clicked();
    void on_modelTestTrainButton_clicked();
    void on_modelTrainButton_clicked();
    void on_datasetChooseButton_clicked();
    void on_imputeButton_clicked();

private:
    Ui::StressSolveAppClass ui;
    QString selectedFilePath;
    DataHandler* dataset;
    StressSolve* stressSolve;

    void loadTable();
    void setTrainingWidgetStates(bool state);
};
