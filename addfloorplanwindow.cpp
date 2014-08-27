#include "addfloorplanwindow.h"
#include "ui_addfloorplanwindow.h"
#include <QFileDialog>
#include <QMessageBox>

AddFloorPlanWindow::AddFloorPlanWindow(QWidget *parent) :
    QDialog(parent), filePath(), filePathSet(false), floorPlanName(), floorPlanLevel(), ui(new Ui::AddFloorPlanWindow) {
    ui->setupUi(this);
}

AddFloorPlanWindow::~AddFloorPlanWindow() {
    delete ui;
}

QString AddFloorPlanWindow::getFloorPlanImageFilePath() {
    return filePath;
}

QString AddFloorPlanWindow::getFloorPlanName() {
    return floorPlanName;
}

QString AddFloorPlanWindow::getFloorPlanLevel() {
    return floorPlanLevel;
}

void AddFloorPlanWindow::on_toolButton_clicked() {
    filePath = QFileDialog::getOpenFileName(this, "Select Floorplan Image", "/Users/Peter/", tr("Image Files (*.png *.jpg *.bmp)") );
    if ( !filePath.isNull() ) {
        ui->filePathLabel->setText(filePath);
        filePathSet = true;
    }
}

void AddFloorPlanWindow::on_okPushButton_clicked() {
    if (ui->floorNameLineEdit->text().isEmpty() || filePathSet == false) {
        QMessageBox e;
        e.setWindowTitle("ERROR");
        if ( ui->floorNameLineEdit->text().isEmpty() ) {
            e.setText("Floor Name cannot be empty\n");
        }
        if (filePathSet == false) {
            e.setText( e.text().append("No file chosen") );
        }
        e.exec();
    } else {
        floorPlanName  = ui->floorNameLineEdit->text();
        floorPlanLevel = ui->levelLineEdit->text();
        accept();
    }
}

void AddFloorPlanWindow::on_cancelPushButton_clicked() {
    reject();
}
