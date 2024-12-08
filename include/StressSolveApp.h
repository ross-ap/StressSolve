#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StressSolveApp.h"

class StressSolveApp : public QMainWindow
{
    Q_OBJECT

public:
    StressSolveApp(QWidget *parent = nullptr);
    ~StressSolveApp();

private:
    Ui::StressSolveAppClass ui;
};
