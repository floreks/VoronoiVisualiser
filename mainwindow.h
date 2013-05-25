#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPainter>

#include <chrono>

#include "Fortune/Voronoi.h"
#include "Fortune/VPoint.h"

#include "Controller/kmeans.h"
#include "Controller/Centroids.h"

enum PaintSwitch {
    KMEANS = 0x01,
    KOHONEN = 0x02,
    NEURALGAS = 0x03,
    NOTHING = 0x00
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *);
private slots:
    void kmeansSwitch();
    void setData();
    void setCentroids();
private:
    Ui::MainWindow *ui;

    Voronoi *v;
    Vertices *ver;
    Vertices *dir;
    Edges *edg;
    double w;

    std::default_random_engine engine;

    QVector<CPoint> centroids;
    QVector<KPoint> points;
    KMeans kmeans;
    QVector<QColor> colors;
    QPainter painter;
    PaintSwitch switcher;

    QColor generateColor();
    void generatePoints();
    VPoint *dRand(double min, double max, std::default_random_engine engine);
    void adjustPoints(double size);

    // KMEANS //
    void paintKMeans(QPainter &painter);
    void kmeansUpdate(QPainter &painter);
};

#endif // MAINWINDOW_H
