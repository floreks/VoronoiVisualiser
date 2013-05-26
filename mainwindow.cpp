#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QShortcut>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(300,50,600,600);
    this->setFixedSize(600,600);
    this->setWindowTitle("Voronoi diagram");

    edg = new Edges();
    ver = new Vertices();
    v = new Voronoi();
    dir = new Vertices();

    w = 590;

    ui->actionKMeans_Algorithm->setEnabled(false);
    ui->actionKohonen_s_Algorithm->setEnabled(false);
    ui->actionNeural_Gas_Algorithm->setEnabled(false);
    ui->actionSet_Data->setEnabled(false);

    connect(ui->actionSet_Data,SIGNAL(triggered()),this,SLOT(setData()));
    connect(ui->actionSet_Centroids,SIGNAL(triggered()),this,SLOT(setCentroids()));
    connect(ui->actionKMeans_Algorithm,SIGNAL(triggered()),this,SLOT(kmeansSwitch()));
    connect(ui->actionKohonen_s_Algorithm,SIGNAL(triggered()),this,SLOT(kohonenSwitch()));
    connect(ui->actionNeural_Gas_Algorithm,SIGNAL(triggered()),this,SLOT(neuralGasSwitch()));

    QShortcut *repaint = new QShortcut(QKeySequence("SPACE"),this);
    connect(repaint,SIGNAL(activated()),this,SLOT(update()));
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
        qDebug() << "KMeans";
        break;
    case(PaintSwitch::KOHONEN):
        qDebug() << "Kohonen";
        break;
    case(PaintSwitch::NEURALGAS):
        qDebug() << "Neural gas";
        break;
    }
}

// ============= UI Signal handling =========== //

void MainWindow::kmeansSwitch() {
    switcher = PaintSwitch::KMEANS;
    kmeans.setCentroids(centroids);
    kmeans.setInputData(points);

    kmeans.start();

    update();
}

void MainWindow::neuralGasSwitch() {
    switcher = PaintSwitch::NEURALGAS;

    update();
}

void MainWindow::kohonenSwitch() {
    switcher = PaintSwitch::KOHONEN;

    update();
}

void MainWindow::setData() {

    points.clear();

    QFile file;
    file.setFileName(QFileDialog::getOpenFileName(this,tr("Open file"),"C:\\Users\\GiBSoN\\Desktop",tr("Text files (*.txt)")));

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QTimer::singleShot(1, qApp, SLOT(quit()));

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

    adjustPoints(w);

    kmeans.setInputData(points);
    kmeans.start();

    ui->actionKMeans_Algorithm->setEnabled(true);
    ui->actionKohonen_s_Algorithm->setEnabled(true);
    ui->actionNeural_Gas_Algorithm->setEnabled(true);

    qDebug() << "Loaded " << points.size() << " points.";
}

void MainWindow::setCentroids() {
    WORD centroidsCount = QInputDialog::getInt(this,tr("Set number of centroids/neurons"),tr("Number of centroids"));
    qDebug() << "Setting " << centroidsCount << " centroids.";

    WORD dimension = QInputDialog::getInt(this,tr("Set number of dimensions"),tr("Dimensions"));
    qDebug() << "Setting " << dimension << " dimensions.";

    centroids = Centroids::randomizeCentroids(centroidsCount,dimension,0,w);

    colors.clear();
    for(int i=0;i<centroidsCount;i++)
        colors.push_back(generateColor());

    kmeans.setCentroids(centroids);
    kmeans.start();
    ui->actionSet_Data->setEnabled(true);
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
        painter.drawEllipse(i.getParams()[0],i.getParams()[1],5,5);
    }

    for(CPoint &i : kmeans.getCentroids()) {
        if(i.getParams()[0] != 0) {
            penColor.setColor(colors[i.getGroup()-1]);
            brushColor.setColor(Qt::black);
            painter.setPen(penColor);
            painter.setBrush(brushColor);
            painter.drawRect(i.getParams()[0],i.getParams()[1],15,15);
        }
    }

    centroids = kmeans.getCentroids();

    for(WORD i=0;i<centroids.size();i++) {
        if(centroids[i].getParams()[0] != 0)
            ver->push_back(new VPoint(centroids[i].getParams()[0],centroids[i].getParams()[1]));
    }
    edg = v->GetEdges(ver, w, w);

    QPen pen(QColor(Qt::red));
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
    painter.fillRect(w-135,18,135,20,Qt::SolidPattern);
    painter.drawText(w-135,35,"Quantization error: " + QString::number(kmeans.getQuantizationError()));

    painter.end();
    ver->clear();
    kmeans.update();
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
        ver->push_back(dRand(0,w,engine));
        dir->push_back(dRand(-1,1,engine));
    }
}

void MainWindow::adjustPoints(double size) {

    // getting min and max
    double min=0,max=0;

    for(int i=0;i<points.size();i++) {
        for(int j=0;j<points[i].getParams().size();j++) {
            if(min > points[i].getParams()[j])
                min = points[i].getParams()[j];
            if(max < points[i].getParams()[j])
                max = points[i].getParams()[j];
        }
    }

    double multiplier = size/(max+abs(min));

    for(int i=0;i<points.size();i++) {
        for(int j=0;j<points[i].getParams().size();j++) {
            points[i].getParams()[j] += abs(min);
            points[i].getParams()[j] *= multiplier;
        }
    }
}

QColor MainWindow::generateColor() {
    engine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dis(0,255);
    return QColor(dis(engine),dis(engine),dis(engine));
}
