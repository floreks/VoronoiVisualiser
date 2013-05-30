#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QShortcut>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QImageReader>
#include <QImage>
#include <QRgb>
#include <QImageWriter>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(300,50,600,600);
    this->setWindowTitle("Self-organizing Maps");

    edg = new Edges();
    ver = new Vertices();
    v = new Voronoi();
    dir = new Vertices();

    nGas = NeuralGas(0.005,0.4,0.01);
    kohonen = Kohonen(4,0.05);

    ui->actionKMeans_Algorithm->setEnabled(false);
    ui->actionKohonen_s_Algorithm->setEnabled(false);
    ui->actionNeural_Gas_Algorithm->setEnabled(false);
    ui->actionSet_Data->setEnabled(false);
    ui->actionExport_Image->setEnabled(false);
    ui->actionLoad_image->setEnabled(false);
    ui->actionNeural_Gas_Image->setEnabled(false);
    ui->graphicsView->setVisible(false);
    ui->graphicsView_2->setVisible(false);

    connect(ui->actionSet_Data,SIGNAL(triggered()),this,SLOT(setData()));
    connect(ui->actionSet_Centroids,SIGNAL(triggered()),this,SLOT(setCentroids()));
    connect(ui->actionKMeans_Algorithm,SIGNAL(triggered()),this,SLOT(kmeansSwitch()));
    connect(ui->actionKohonen_s_Algorithm,SIGNAL(triggered()),this,SLOT(kohonenSwitch()));
    connect(ui->actionNeural_Gas_Algorithm,SIGNAL(triggered()),this,SLOT(neuralGasSwitch()));
    connect(ui->actionLoad_image,SIGNAL(triggered()),this,SLOT(loadImage()));
    connect(ui->actionExport_Image,SIGNAL(triggered()),this,SLOT(exportImage()));
    connect(ui->actionNeural_Gas_Image,SIGNAL(triggered()),this,SLOT(neuralGasImageSwitch()));

    QShortcut *repaint = new QShortcut(QKeySequence("SPACE"),this);
    connect(repaint,SIGNAL(activated()),this,SLOT(doAction()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete edg;
    delete ver;
    delete dir;
    delete v;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    switch(switcher) {
    case(PaintSwitch::KMEANS):
        kmeansUpdate(painter);
        break;
    case(PaintSwitch::KOHONEN):
        kohonenUpdate(painter);
        break;
    case(PaintSwitch::NEURALGAS):
        neuralGasUpdate(painter);
        break;
    case(PaintSwitch::NOTHING):
        break;
    }
}

void MainWindow::resizeEvent(QResizeEvent *) {
    kmeans.normalizePoints(this->height());
    kmeans.start();

    nGas.normalizePoints(this->height());
    nGas.start();

    kohonen.normalizePoints(this->height());
    kohonen.start();
}

// ============= UI Signal handling =========== //

void MainWindow::kmeansSwitch() {
    switcher = PaintSwitch::KMEANS;
    kmeans.start();

    update();
}

void MainWindow::neuralGasSwitch() {
    switcher = PaintSwitch::NEURALGAS;
    nGas.start();

    update();
}

void MainWindow::kohonenSwitch() {
    switcher = PaintSwitch::KOHONEN;
    kohonen.start();

    update();
}

void MainWindow::neuralGasImageSwitch() {
    nGas.start();
    this->setEnabled(false);
    if(nGas.update(this))
        ui->actionExport_Image->setEnabled(true);
    this->setEnabled(true);
}

void MainWindow::setData() {

    switcher = PaintSwitch::NOTHING;

    QVector<KPoint> points;

    QFile file;
    file.setFileName(QFileDialog::getOpenFileName(this,tr("Open file"),"C:\\Users\\GiBSoN\\Desktop",tr("Text files (*.txt)")));

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QVector<double> params;
        QStringList list = line.split(" ");
        for(QString i : list)
            params.push_back(i.toDouble());
        points.push_back(params);
        params.clear();
    }

    kmeans.setInputData(points);
    kmeans.normalizePoints(this->height()-10);

    nGas.setInputData(points);
    nGas.normalizePoints(this->height()-10);

    kohonen.setInputData(points);
    kohonen.normalizePoints(this->height()-10);

    ui->actionKMeans_Algorithm->setEnabled(true);
    ui->actionKohonen_s_Algorithm->setEnabled(true);
    ui->actionNeural_Gas_Algorithm->setEnabled(true);

    qDebug() << "Loaded " << points.size() << " points.";
}

void MainWindow::loadImage() {

    QImageReader reader;
    reader.setFileName(QFileDialog::getOpenFileName(this,tr("Open image"),"C:\\Users\\GiBSoN\\Desktop",tr("Image files (*.jpg *.png)")));
    QImage image = reader.read();
    imageSize = image.size();

    QVector<double> params;
    QVector<KPoint> points;

    for(int i=0;i<image.width();i++) {
        for(int j=0;j<image.height();j++) {
            params.push_back(qRed(image.pixel(i,j)));
            params.push_back(qGreen(image.pixel(i,j)));
            params.push_back(qBlue(image.pixel(i,j)));
            points.push_back(params);
            params.clear();
        }
    }

    nGas.setInputData(points);

    ui->actionNeural_Gas_Image->setEnabled(true);

    ui->graphicsView->setVisible(true);
    ui->graphicsView_2->setVisible(true);

    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->addPixmap(QPixmap::fromImage(image.scaled(ui->graphicsView->width()*0.95,ui->graphicsView->height()*0.95)));
    ui->graphicsView->show();

    qDebug() << "Pixels: " << points.size() << " Dimensions: " << points[0].getDimensions();
}

// ================= COMPARATOR =================== //

class SortByDistance {
public:
    bool operator() (CPoint i, CPoint j) {
        return i.getDistance() < j.getDistance();
    }
};

void MainWindow::exportImage() {

    QVector<CPoint> centroids = nGas.getCentroids();
    QVector<KPoint> points = nGas.getPoints();

    QImage image(imageSize,QImage::Format_RGB32);

    QProgressDialog progress(this);
    progress.setWindowTitle("Self-organizing maps");
    progress.setLabelText("Generating file...");
    progress.setModal(true);
    progress.show();

    int k = 0;

    for(int i=0;i<image.width();i++) {
        progress.setValue(((double)i / (double)image.width()) * 100);
        if(progress.wasCanceled())
            return;

        for(int j=0;j<image.height();j++) {
            for(int d=0;d<centroids.size();d++)
                centroids[d].setDistance(Centroids::countDistance(centroids[d],points[k]));
            qSort(centroids.begin(),centroids.end(),SortByDistance());
            image.setPixel(i,j,qRgb(centroids[0].paramAt(0),centroids[1].paramAt(1),centroids[2].paramAt(2)));
            k++;
        }
    }

    progress.setValue(100);

    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->graphicsView_2->setScene(scene);
    scene->addPixmap(QPixmap::fromImage(image.scaled(ui->graphicsView->width()*0.95,ui->graphicsView->height()*0.95)));
    ui->graphicsView_2->show();

    QImageWriter writer;
    writer.setFileName(QFileDialog::getSaveFileName(this,tr("Open image"),"C:\\Users\\GiBSoN\\Desktop",tr("Image files (*.jpg *.png)")));
    writer.setFormat("jpg");
    writer.setCompression(10);
    writer.write(image);
}

void MainWindow::setCentroids() {

    switcher = PaintSwitch::NOTHING;

    QVector<CPoint> centroids;

    WORD centroidsCount = QInputDialog::getInt(this,tr("Set number of centroids/neurons"),tr("Number of centroids"));
    qDebug() << "Setting " << centroidsCount << " centroids.";

    WORD dimension = QInputDialog::getInt(this,tr("Set number of dimensions"),tr("Dimensions"));
    qDebug() << "Setting " << dimension << " dimensions.";

    centroids = Centroids::randomizeCentroids(centroidsCount,dimension,50,this->height()-50);

    colors.clear();
    for(int i=0;i<centroidsCount;i++)
        colors.push_back(generateColor());

    kmeans.setCentroids(centroids);
    nGas.setCentroids(centroids);
    kohonen.setCentroids(centroids);
    if(dimension == 2) {
        ui->actionSet_Data->setEnabled(true);
        ui->actionLoad_image->setEnabled(false);
        ui->graphicsView->setVisible(false);
        ui->graphicsView_2->setVisible(false);
    }
    if(dimension == 3) {
        ui->actionLoad_image->setEnabled(true);
        ui->actionSet_Data->setEnabled(false);
        ui->actionKMeans_Algorithm->setEnabled(false);
        ui->actionKohonen_s_Algorithm->setEnabled(false);
        ui->actionNeural_Gas_Algorithm->setEnabled(false);
    }
}

// ============= KMEANS Section ==================== //

void MainWindow::kmeansUpdate(QPainter &painter) { // paints updated kmeans + voronoi

    QPen penColor;
    QBrush brushColor(Qt::SolidPattern);
    QPainterPath path;

    painter.begin(this);

    for(KPoint &i : kmeans.getPoints()) {
        penColor.setColor(colors[i.getGroup()-1]);
        brushColor.setColor(colors[i.getGroup()-1]);
        painter.setPen(penColor);
        painter.setBrush(brushColor);
        painter.drawEllipse(i.paramAt(0),i.paramAt(1),5,5);
    }

    for(CPoint &i : kmeans.getCentroids()) {
        if(i.paramAt(0) != 0) {
            penColor.setColor(colors[i.getGroup()-1]);
            brushColor.setColor(Qt::black);
            painter.setPen(penColor);
            painter.setBrush(brushColor);
            painter.drawRect(i.paramAt(0)-4,i.paramAt(1)-4,8,8);
        }
    }

    // Voronoi drawing

    for(WORD i=0;i<kmeans.getCentroids().size();i++) {
        if(kmeans.getCentroids()[i].paramAt(0) != 0)
            ver->push_back(new VPoint(kmeans.getCentroids()[i].paramAt(0),kmeans.getCentroids()[i].paramAt(1)));
    }
    edg = v->GetEdges(ver, this->width(), this->height());

    QPen pen(QColor(Qt::black));
    pen.setWidth(2);
    painter.setPen(pen);
    for(Edges::iterator i = edg->begin(); i!= edg->end(); ++i)
    {
        path.moveTo((*i)->start->x,  (*i)->start->y);
        path.lineTo((*i)->end->x, (*i)->end->y);
        painter.drawPath(path);
        path = QPainterPath();
    }

    painter.setPen(QPen(QColor(Qt::white)));
    painter.fillRect(this->width()-200,18,200,20,Qt::SolidPattern);
    painter.drawText(this->width()-200,35,"Quantization error: " + QString::number(kmeans.getQuantizationError()));

    painter.end();
    ver->clear();
}

// =================== Neural Gas Section ================== //

void MainWindow::neuralGasUpdate(QPainter &painter) {

    QPainterPath path;
    QPen penColor;
    QBrush brushColor(Qt::SolidPattern);

    painter.begin(this);

    for(KPoint &i : nGas.getPoints()) {
        penColor.setColor(QColor(Qt::gray));
        brushColor.setColor(Qt::gray);
        painter.setBrush(brushColor);
        painter.setPen(penColor);
        painter.drawEllipse(i.paramAt(0),i.paramAt(1),5,5);
    }

    for(CPoint &i : nGas.getCentroids()) {
        brushColor.setColor(Qt::black);
        penColor.setColor(Qt::black);
        painter.setPen(penColor);
        painter.setBrush(brushColor);
        painter.drawRect(i.paramAt(0)-4,i.paramAt(1)-4,8,8);
    }

    // Voronoi drawing

    for(WORD i=0;i<nGas.getCentroids().size();i++) {
        if(nGas.getCentroids()[i].paramAt(0) != 0)
            ver->push_back(new VPoint(nGas.getCentroids()[i].paramAt(0),nGas.getCentroids()[i].paramAt(1)));
    }
    edg = v->GetEdges(ver, this->width(), this->height());

    QPen pen(QColor(Qt::black));
    pen.setWidth(2);
    painter.setPen(pen);
    for(Edges::iterator i = edg->begin(); i!= edg->end(); ++i)
    {
        path.moveTo((*i)->start->x,  (*i)->start->y);
        path.lineTo((*i)->end->x, (*i)->end->y);
        painter.drawPath(path);
        path = QPainterPath();
    }

    painter.setPen(QPen(QColor(Qt::white)));
    painter.fillRect(this->width()-200,18,200,20,Qt::SolidPattern);
    painter.drawText(this->width()-200,35,"Quantization error: " + QString::number(nGas.getQuantizationError()));

    painter.end();
    ver->clear();
}

// ================ Kohonen section =================== //

void MainWindow::kohonenUpdate(QPainter &painter) {

    QPainterPath path;
    QPen penColor;
    QBrush brushColor(Qt::SolidPattern);

    painter.begin(this);

    for(KPoint &i : kohonen.getPoints()) {
        penColor.setColor(QColor(Qt::gray));
        brushColor.setColor(QColor(Qt::gray));
        painter.setPen(penColor);
        painter.setBrush(brushColor);
        painter.drawEllipse(i.paramAt(0),i.paramAt(1),5,5);
    }

    for(CPoint &i : kohonen.getCentroids()) {
        penColor.setColor(Qt::black);
        brushColor.setColor(Qt::black);
        painter.setPen(penColor);
        painter.setBrush(brushColor);
        painter.drawRect(i.paramAt(0)-4,i.paramAt(1)-4,8,8);
    }

    // Voronoi drawing

    for(WORD i=0;i<kohonen.getCentroids().size();i++) {
        if(kohonen.getCentroids()[i].paramAt(0) != 0)
            ver->push_back(new VPoint(kohonen.getCentroids()[i].paramAt(0),kohonen.getCentroids()[i].paramAt(1)));
    }
    edg = v->GetEdges(ver, this->width(), this->height());

    QPen pen(QColor(Qt::black));
    pen.setWidth(2);
    painter.setPen(pen);
    for(Edges::iterator i = edg->begin(); i!= edg->end(); ++i)
    {
        path.moveTo((*i)->start->x,  (*i)->start->y);
        path.lineTo((*i)->end->x, (*i)->end->y);
        painter.drawPath(path);
        path = QPainterPath();
    }

    painter.setPen(QPen(QColor(Qt::white)));
    painter.fillRect(this->width()-200,18,200,20,Qt::SolidPattern);
    painter.drawText(this->width()-200,35,"Quantization error: " + QString::number(kohonen.getQuantizationError()));

    painter.end();
    ver->clear();
}

void MainWindow::doAction() {
    switch(switcher) {
    case(PaintSwitch::KMEANS):
        kmeans.update();
        update();
        break;
    case(PaintSwitch::KOHONEN):
        kohonen.update(this);
        update();
        break;
    case(PaintSwitch::NEURALGAS):
        nGas.update(this);
        update();
        break;
    case(PaintSwitch::NOTHING):
        break;
    }
}

// ============= PRIVATE ================ //

VPoint* MainWindow::dRand(double min, double max, std::default_random_engine engine)
{
    engine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dis(min,max);
    VPoint *p = new VPoint(dis(engine),dis(engine));
    return p;
}

void MainWindow::generatePoints()
{
    for(int i=0; i<50; i++)
    {
        ver->push_back(dRand(0,this->height(),engine));
        dir->push_back(dRand(-1,1,engine));
    }
}

QColor MainWindow::generateColor() {
    engine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dis(0,255);
    return QColor(dis(engine),dis(engine),dis(engine));
}
