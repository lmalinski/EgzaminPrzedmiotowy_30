#define MAIN

#include "mainwindow.h"

#ifdef MAIN
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

#endif

#ifdef DEBUG

#include "urn.h"
#include <QVector>
#include <QDebug>
int main()
{
    Urn urna(10);
    qInfo() << urna.losuj(3);


}



#endif
