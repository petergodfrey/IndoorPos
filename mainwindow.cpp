#include <QVector>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logger.h"
#include "databasedriver.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    c  = new Configurator();
    db = new DatabaseDriver( c->databaseAddress(), c->databasePort(), c->databaseName() );
    l  = new Logger(c->hostAddress(), c->hostPort(), db, this);

    QVector< QPair<QString, int> > b = db->getBuildings();
    for (QVector< QPair<QString, int> >::iterator it = b.begin(); it != b.end(); it++) {
        ui->buildingComboBox->addItem(it->first, it->second);
    }
}

MainWindow::~MainWindow()
{
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
    ui->floorPlanComboBox->clear();
    QVector< QPair<QString, int> > b = db->getFloorplanNames( ui->buildingComboBox->itemData(index).toInt() );
    for (QVector< QPair<QString, int> >::iterator it = b.begin(); it != b.end(); it++) {
         ui->floorPlanComboBox->addItem(it->first, it->second);
    }
}

void MainWindow::on_floorPlanComboBox_currentIndexChanged(int index) {
    l->setFloorPlanID( ui->floorPlanComboBox->itemData(index).toInt() );
}
