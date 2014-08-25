#include <QVector>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logger.h"
#include "databasedriver.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    c = new Configurator();
    DatabaseDriver *db = new DatabaseDriver( c->databaseAddress(), c->databasePort(), c->databaseName() );
    l = new Logger(c->hostAddress(), c->hostPort(), db, this);

    // Set up Tree
    QVector< QPair<QString, int> > b = db->getBuildings();
    for (QVector< QPair<QString, int> >::iterator bIt = b.begin(); bIt != b.end(); bIt++) {
        QTreeWidgetItem *i = new QTreeWidgetItem(ui->treeWidget);
        i->setText(0, bIt->first);
        QStringList f = db->getFloorplanNames(bIt->second);
        for (QStringList::iterator fIt = f.begin(); fIt != f.end(); fIt++) {
            QTreeWidgetItem *j = new QTreeWidgetItem();
            j->setText(0, *fIt);
            i->addChild(j);
        }
    }
    l->setFloorPlanID(ui->floorPlanIDLineEdit->text().toInt());
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
