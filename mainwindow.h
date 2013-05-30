#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPainter>
#include <QProgressDialog>

#include <chrono>

#include "Fortune/Voronoi.h"
#include "Fortune/VPoint.h"

#include "Controller/kmeans.h"
#include "Controller/Centroids.h"
#include "Controller/neuralgas.h"
#include "Controller/kohonen.h"

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
    void resizeEvent(QResizeEvent *);
private slots:
    void kmeansSwitch();
    void kohonenSwitch();
    void neuralGasSwitch();
    void setData();
    void setCentroids();
    void doAction();
    void loadImage();
    void exportImage();
    void neuralGasImageSwitch();
private:
    Ui::MainWindow *ui;

    Voronoi *v;
    Vertices *ver;
    Vertices *dir;
    Edges *edg;

    std::default_random_engine engine;

    KMeans kmeans;
    NeuralGas nGas;
    Kohonen kohonen;
    QVector<QColor> colors;
    QPainter painter;
    PaintSwitch switcher;
    QSize imageSize;

    QColor generateColor();
    void generatePoints();
    VPoint *dRand(double min, double max, std::default_random_engine engine);

    void kmeansUpdate(QPainter &painter);
    void neuralGasUpdate(QPainter &painter);
    void kohonenUpdate(QPainter &painter);
};

#endif // MAINWINDOW_H
