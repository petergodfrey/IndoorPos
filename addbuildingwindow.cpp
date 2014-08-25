#include "addbuildingwindow.h"
#include "ui_addbuildingwindow.h"
#include <QDebug>
#include <QMessageBox>

AddBuildingWindow::AddBuildingWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AddBuildingWindow) {
    ui->setupUi(this);
}

AddBuildingWindow::~AddBuildingWindow() {
    delete ui;
}

QString AddBuildingWindow::getBuildingName(void) {
    return buildingName;
}

QString AddBuildingWindow::getBuildingAddress(void) {
    return buildingAddress;
}

void AddBuildingWindow::on_OKPushButton_clicked() {
    if ( ui->buildingNameLineEdit->text().isEmpty() ) {
        QMessageBox e(this);
        e.setText("Building Name cannot be empty");
        e.exec();
    } else {
        buildingName    = ui->buildingNameLineEdit->text();
        buildingAddress = ui->buildingAddressLineEdit->text();
        this->accept();
    }
}

void AddBuildingWindow::on_CancelPushButton_clicked() {
    this->destroy();
}
