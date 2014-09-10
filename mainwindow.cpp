#include <QDebug>
#include <QMessageBox>

#include "mainwindow.h"
#include "addbuildingwindow.h"
#include "addfloorplanwindow.h"
#include "ui_mainwindow.h"
#include "Exceptions/databaseexception.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    c  = new Configurator();
    db = new DatabaseDriver( c->databaseAddress(), c->databasePort(), c->databaseName() );
    s  = new QTcpSocket(this);
    s->connectToHost( c->hostAddress(), c->hostPort() );
    if ( s->waitForConnected(1000) ) {
        qDebug() << "Connected to host";
    } else {
        qDebug() << "Connection to host failed";
    }
    l  = new Logger(s, db, this);
    m  = new Matcher(s, db);

    positioningThread = new QThread();
    m->moveToThread(positioningThread);
    connect(positioningThread, SIGNAL( started() ),               m,                 SLOT( start() ) );
    connect(m,                 SIGNAL( locationChanged(QPoint) ), this,              SLOT( positioningLocationChanged(QPoint) ) );

    //connect(m,                 SIGNAL( locationChanged(QPoint) ), positioningThread, SLOT( quit() ) );
    //connect(m,                 SIGNAL( locationChanged(QPoint) ), m,                 SLOT( deleteLater() ) );
    //connect(positioningThread, SIGNAL( finished() ),              positioningThread, SLOT( deleteLater() ) );
    //timer = new QTimer();
    //connect(timer, SIGNAL( timeout() ), positioningThread, SLOT( start() ) );


    ui->tabWidget->setCurrentIndex(0);

    loggingImageViewer = new ImageViewer(this);
    loggingImageViewer->setWidgetResizable(true);
    ui->verticalLayout->addWidget(loggingImageViewer);

    positioningImageViewer = new ImageViewer(this);
    positioningImageViewer->setWidgetResizable(true);
    ui->verticalLayout_4->addWidget(positioningImageViewer);

    QSqlQueryModel *buildingsModel = db->buildingsModel();
    ui->positioningBuildingComboBox->setModel(buildingsModel);
    ui->loggingBuildingComboBox->setModel(buildingsModel);
}

MainWindow::~MainWindow() {
    delete ui;
    delete l;
    delete s;
    delete c;
    delete m;
    delete loggingImageViewer;
    delete positioningImageViewer;
}


/********************************************************************************/
/* SLOTS */
/********************************************************************************/

void MainWindow::on_tabWidget_currentChanged(int index) {
    if (index == 1) { // Positioning tab

    } else if (index == 0) {

    }
}


/********************************************************************************/
/* Logging Tab */

void MainWindow::on_loggingBuildingComboBox_currentIndexChanged(int index) {
    ui->loggingFloorPlanComboBox->setModel( db->loggingFloorPlansModel(index) );
}

void MainWindow::on_loggingFloorPlanComboBox_currentIndexChanged(int index) {
    int floorPlan = db->loggingFloorPlanID(index);
    l->setFloorPlanID(floorPlan);
    loggingImageViewer->open( db->floorPlanImagePath(floorPlan) );
}

void MainWindow::on_newBuildingPushButton_clicked() {
    AddBuildingWindow w;
    w.setWindowTitle("Add New Building");
    if (w.exec() == QDialog::Accepted) {
        qDebug() << "Accepted";
        db->addBuilding( w.getBuildingName(), w.getBuildingAddress() );
    }
}

void MainWindow::on_newFloorPlanPushButton_clicked() {
    AddFloorPlanWindow w(ui->loggingBuildingComboBox);
    w.setWindowTitle("Add New Floor Plan");
    if (w.exec() == QDialog::Accepted) {
        QString oldFilePath( w.getFloorPlanImageFilePath() );
        QStringList l = oldFilePath.split(".");
        QString extension =( l.back() );
        QString newFilePath( QDir::currentPath() );
        newFilePath.append("/FloorPlanImages/%1-%2-%3-%4.%5"
            ).arg( w.getFloorPlanName() ).arg( w.getFloorPlanLevel() ).arg( db->buildingID( ui->loggingBuildingComboBox->currentIndex() ) ).arg( time(0) ).arg(extension);
        if (QFile::copy(oldFilePath, newFilePath) == false) {
            qDebug() << oldFilePath;
            qDebug() << newFilePath;
            qDebug() << "File copy failed";
        }
        db->addFloorplan( db->buildingID( ui->loggingBuildingComboBox->currentIndex() ), w.getFloorPlanName(), w.getFloorPlanLevel(), newFilePath);
    }
}

void MainWindow::on_buildingDeletePushButton_clicked() {

}

void MainWindow::on_floorPlanDeletePushButton_clicked() {
    db->deleteFloorPlan( db->loggingFloorPlanID( ui->loggingFloorPlanComboBox->currentIndex() ) );
}

void MainWindow::on_loggingStartButton_clicked() {
    if ( !loggingImageViewer->pointsAreSet() ) {
        QMessageBox::warning(this, tr("ERROR"), tr("Start & end points must be selected"));
    } else {
        if (l->stop == true) { // Only proceed if thread has been stopped
            ui->dataLoggingLabel->setText("Now logging data.....");
            l->setStartPoint( loggingImageViewer->startPoint() );
            l->setEndPoint( loggingImageViewer->endPoint() );
            l->stop = false;
            l->start();
        }
    }
}

void MainWindow::on_loggingStopButton_clicked() {
    if (l->stop == false) {
        l->stop = true;
        ui->dataLoggingLabel->setText("");
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question( this, tr("Are you sure?"),
                                       tr("Are you sure you want to save this data\nThis step cannot be undone") );
        if (reply == QMessageBox::Yes) {
            l->commit();
            loggingImageViewer->paintLineLogged();
            // TODO Save image to disk
        }
    }
}

void MainWindow::on_loggingZoomInPushButton_clicked() {
    loggingImageViewer->zoomIn();
}

void MainWindow::on_loggingZoomOutPushButton_clicked() {
    loggingImageViewer->zoomOut();
}


/********************************************************************************/
/* Positioning Tab */

void MainWindow::on_positioningBuildingComboBox_currentIndexChanged(int index) {
    ui->positioningFloorPlanComboBox->setModel( db->positioningFloorPlansModel(index) );
}

void MainWindow::on_positioningFloorPlanComboBox_currentIndexChanged(int index) {
    int floorPlan = db->positioningFloorPlanID(index);
    positioningImageViewer->open( db->floorPlanImagePath(floorPlan) );
    m->setFloorPlanID(floorPlan);
}

void MainWindow::on_positioningZoomInPushButton_clicked() {
    positioningImageViewer->zoomIn();
}

void MainWindow::on_positioningZoomOutPushButton_clicked() {
    positioningImageViewer->zoomOut();
}

void MainWindow::on_positioningStartButton_clicked() {
    if ( positioningThread->isRunning() ) {
        m->start();
    } else {
        positioningThread->start();
    }
}

void MainWindow::on_positioningStopButton_clicked() {
    m->stop();
}

void MainWindow::positioningLocationChanged(const QPoint &location) {
    positioningImageViewer->paintLocation(location);
}

/********************************************************************************/
