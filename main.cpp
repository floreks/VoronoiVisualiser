#include "mainwindow.h"
#include <QApplication>
#include <Controller/Centroids.h>
#include <QDebug>
#include <Controller/kmeans.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
