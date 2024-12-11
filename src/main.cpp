#include "stdafx.h"
#include "StressSolveApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StressSolveApp w;
    w.show();
    return a.exec();
}
