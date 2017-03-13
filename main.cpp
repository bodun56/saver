#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setLayout(w.vblayout);
    w.show();

    return a.exec();
}
