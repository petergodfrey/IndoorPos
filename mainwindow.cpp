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
    if ( db->isConnected() ) {
        qDebug() << "Connected to database";
    } else {
        qDebug() << "Connection to database failed";
    }

    s = new QTcpSocket(this);
    s->connectToHost( c->hostAddress(), c->hostPort() );
    if ( s->waitForConnected(1000) ) {
        qDebug() << "Connected to host";
    } else {
        qDebug() << "Connection to host failed";
    }

    l = new Logger(s, db);
    l->moveToThread(&loggingThread);
    connect(this, SIGNAL( startLogging(QPoint, QPoint, int) ),
            l,    SLOT  ( start(QPoint, QPoint, int) ) );
    connect(this, SIGNAL( stopLogging() ),
            l,    SLOT  ( stop() ) );
    loggingThread.start();

    m = new Matcher(s, db);
    m->moveToThread(&positioningThread);
    connect(this, SIGNAL( startPositioning(int) ),
            m,    SLOT  (start(int) ) );
    connect(this, SIGNAL( stopPositioning() ),
            m,    SLOT  ( stop() ) );
    connect(m,    SIGNAL( locationChanged(QPoint) ),
            this, SLOT  ( positioningLocationChanged(QPoint) ) );
    positioningThread.start();

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
    loggingThread.quit();
    loggingThread.wait();
    positioningThread.quit();
    positioningThread.wait();
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
        l->stop();
    } else if (index == 0) {
        m->stop();
    }
}


/********************************************************************************/
/* Logging Tab */

void MainWindow::on_loggingBuildingComboBox_currentIndexChanged(int index) {
    ui->loggingFloorPlanComboBox->setModel( db->loggingFloorPlansModel(index) );
}

void MainWindow::on_loggingFloorPlanComboBox_currentIndexChanged(int index) {
    int floorPlan = db->loggingFloorPlanID(index);
    loggingImageViewer->open( db->floorPlanImagePath(floorPlan), db->floorPlanWidth(floorPlan) );
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
        QString newFilePath("/Users/Peter/Developer/Qt Projects/GeoPosition"); //QDir::currentPath() );
        newFilePath.append( QString("/FloorPlanImages/%1-%2-%3-%4.%5"
                 ).arg( w.getFloorPlanName()
                 ).arg( w.getFloorPlanLevel()
                 ).arg( db->buildingID( ui->loggingBuildingComboBox->currentIndex() )
                 ).arg( time(0)
                 ).arg(extension) );
        if (QFile::copy(oldFilePath, newFilePath) == false) {
            qDebug() << oldFilePath;
            qDebug() << newFilePath;
            qDebug() << "File copy failed";
        }
        db->addFloorplan( db->buildingID( ui->loggingBuildingComboBox->currentIndex() ),
                          w.getFloorPlanName(),
                          w.getFloorPlanLevel(),
                          newFilePath,
                          w.getWidth() );
    }
}

void MainWindow::on_buildingDeletePushButton_clicked() {

}

void MainWindow::on_floorPlanDeletePushButton_clicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question( this,
                                   tr("Are you sure?"),
                                   tr("Are you sure you want to delete the selected floorplan and all it's data?") );
    if (reply == QMessageBox::Yes) {
        db->deleteFloorPlan( db->loggingFloorPlanID( ui->loggingFloorPlanComboBox->currentIndex() ) );
    }
}

void MainWindow::on_loggingStartButton_clicked() {
    if ( !loggingImageViewer->pointsAreSet() ) {
        QMessageBox::warning(this, tr("ERROR"), tr("Start & end points must be selected"));
    } else {
        if ( !l->isRunning() ) { // Only proceed if thread has been stopped
            ui->dataLoggingLabel->setText("Now logging data.....");
            int floorPlan =  db->loggingFloorPlanID( ui->loggingFloorPlanComboBox->currentIndex() );
            emit startLogging(loggingImageViewer->startPoint(), loggingImageViewer->endPoint(), floorPlan);
        }
    }
}

void MainWindow::on_loggingStopButton_clicked() {
    if ( l->isRunning() ) {
        l->stop();
        //emit stopLogging();
        ui->dataLoggingLabel->setText("");
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question( this,
                                       tr("Are you sure?"),
                                       tr("Are you sure you want to save this data\nThis step cannot be undone") );
        if (reply == QMessageBox::Yes) {
            l->commit();
            loggingImageViewer->paintLineLogged();
            // TODO POSSIBLY: Save image to disk
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
    positioningImageViewer->open( db->floorPlanImagePath(floorPlan), db->floorPlanWidth(floorPlan) );
}

void MainWindow::on_positioningZoomInPushButton_clicked() {
    positioningImageViewer->zoomIn();
}

void MainWindow::on_positioningZoomOutPushButton_clicked() {
    positioningImageViewer->zoomOut();
}

void MainWindow::on_positioningStartButton_clicked() {
    emit startPositioning( db->positioningFloorPlanID( ui->positioningFloorPlanComboBox->currentIndex() ) );
}

void MainWindow::on_positioningStopButton_clicked() {
    emit stopPositioning();
}

void MainWindow::positioningLocationChanged(const QPoint &location) {
    qDebug() << location;
    positioningImageViewer->paintLocation(location);
}

/********************************************************************************/
