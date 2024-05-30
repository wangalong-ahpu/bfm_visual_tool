#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "Functions.h"
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
