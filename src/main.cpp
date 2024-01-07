#include "MainWindow.hpp"

#include <QApplication>
#include <QLocale>
#include <cstdint>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;

    window.show();
    return a.exec();
}
