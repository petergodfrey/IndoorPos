#include <QVector>
#include <QDir>
#include <QMessageBox>
#include <ctime>
#include "addbuildingwindow.h"
#include "addfloorplanwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logger.h"
#include "databasedriver.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    c  = new Configurator();
    db = new DatabaseDriver( c->databaseAddress(), c->databasePort(), c->databaseName() );
    l  = new Logger(c->hostAddress(), c->hostPort(), db, this);

    updateBuildingComboBox();
}

MainWindow::~MainWindow() {
    delete ui;
    delete l;
    delete c;
}

void MainWindow::on_startButton_clicked() {
    if (l->stop == true) { // Only proceed if thread has been stopped
        l->stop   = false;
        l->xStart = ui->xStartLineEdit->text().toInt();
        l->xEnd   = ui->xEndLineEdit->text().toInt();
        l->yStart = ui->yStartLineEdit->text().toInt();
        l->yEnd   = ui->yEndLineEdit->text().toInt();
        l->start();
    }
}

void MainWindow::on_stopButton_clicked() {
    l->stop = true;
}

void MainWindow::on_buildingComboBox_currentIndexChanged(int index) {
    (void)index; // Silence unused variable compiler warning
    updateFloorPlanComboBox();
}

void MainWindow::on_floorPlanComboBox_currentIndexChanged(int index) {
    l->setFloorPlanID( ui->floorPlanComboBox->itemData(index).toInt() );
}


void MainWindow::on_addNewBuildingPushButton_clicked() {
    AddBuildingWindow w;
    w.setWindowTitle("Add New Building");
    if (w.exec() == QDialog::Accepted) {
        db->addBuilding( w.getBuildingName(), w.getBuildingAddress() );
        updateBuildingComboBox();
    }
}

// Clear the building combo box & reload values from the database
void MainWindow::updateBuildingComboBox() {
    ui->buildingComboBox->clear();
    QVector< QPair<QString, int> > b = db->getBuildings();
    for (QVector< QPair<QString, int> >::iterator it = b.begin(); it != b.end(); it++) {
        ui->buildingComboBox->addItem(it->first, it->second);
    }
}

void MainWindow::updateFloorPlanComboBox() {
    ui->floorPlanComboBox->clear();
    QVector< QPair<QString, int> > b = db->getFloorplanNames( ui->buildingComboBox->currentData().toInt() );
    for (QVector< QPair<QString, int> >::iterator it = b.begin(); it != b.end(); it++) {
         ui->floorPlanComboBox->addItem(it->first, it->second);
    }
}

void MainWindow::on_addNewFloorPlanPushButton_clicked() {
    AddFloorPlanWindow w(ui->buildingComboBox);
    w.setWindowTitle("Add New Floor Plan");
    if (w.exec() == QDialog::Accepted) {
        QString oldFilePath( w.getFloorPlanImageFilePath() );
        QStringList l = oldFilePath.split(".");
        QString extension =( l.back() );
        QString newFilePath = QString("/Users/Peter/Developer/Qt\ Projects/GeoPosition/FloorPlanImages/%1-%2-%3-%4.%5").arg( w.getFloorPlanName() ).arg( w.getFloorPlanLevel() ).arg( ui->buildingComboBox->currentData().toInt() ).arg( time(0) ).arg(extension);
        if (QFile::copy(oldFilePath, newFilePath) == false) {
            qDebug() << oldFilePath;
            qDebug() << newFilePath;
            qDebug() << "File copy failed";
        }
        db->addFloorplan(ui->buildingComboBox->currentData().toInt(), w.getFloorPlanName(), w.getFloorPlanLevel(), newFilePath);
        updateFloorPlanComboBox();
    }
}
