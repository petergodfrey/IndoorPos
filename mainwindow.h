#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logger.h"
#include "configurator.h"
#include "imageviewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int             buildingID;
    Logger         *l;
    Configurator   *c;
    DatabaseDriver *db;

private:
    ImageViewer *imageViewer;
    void updateBuildingComboBox();
    void updateFloorPlanComboBox();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_buildingComboBox_currentIndexChanged(int index);
    void on_floorPlanComboBox_currentIndexChanged(int index);
    void on_addNewBuildingPushButton_clicked();
    void on_addNewFloorPlanPushButton_clicked();
    void on_zoomInPushButton_clicked();
    void on_zoomOutPushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
