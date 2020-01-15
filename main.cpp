#include <QApplication>
#include "mainwindow.h"
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator qtTranslator;
       qtTranslator.load("qt_" + QLocale::system().name(),
               QLibraryInfo::location(QLibraryInfo::TranslationsPath));
       a.installTranslator(&qtTranslator);

    MainWindow w;

    w.show();

    return a.exec();
}
