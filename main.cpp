#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("Модель распространения городской пыльцы");
    w.show();
    return app.exec();
}
