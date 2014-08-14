#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>

#include "reader.h"
#include "logger.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    l = new Logger(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked() {
    if (l->stop == true) { // Only proceed if thread has been stopped
        l->stop = false;
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
