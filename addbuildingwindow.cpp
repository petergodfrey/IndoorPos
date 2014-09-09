#include "addbuildingwindow.h"
#include "ui_addbuildingwindow.h"
#include <QDebug>
#include <QMessageBox>

AddBuildingWindow::AddBuildingWindow(QWidget *parent) : buildingName(""), buildingAddress(""), QDialog(parent), ui(new Ui::AddBuildingWindow) {
    ui->setupUi(this);
}

AddBuildingWindow::~AddBuildingWindow() {
    delete ui;
}

QString AddBuildingWindow::getBuildingName() {
    return buildingName;
}

QString AddBuildingWindow::getBuildingAddress() {
    return buildingAddress;
}

void AddBuildingWindow::on_OKPushButton_clicked() {
    if ( ui->buildingNameLineEdit->text().isEmpty() ) {
        QMessageBox::warning( this, tr("ERROR"), tr("Building name cannot be empty") );
    } else {
        buildingName    = ui->buildingNameLineEdit->text();
        buildingAddress = ui->buildingAddressLineEdit->text();
        accept();
    }
}

void AddBuildingWindow::on_CancelPushButton_clicked() {
    reject();
}
