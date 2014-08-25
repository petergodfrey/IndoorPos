#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logger.h"
#include "configurator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int floorPlanID;
    // Data logger thread
    Logger *l;
    Configurator *c;
    DatabaseDriver *db;

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();

    void on_buildingComboBox_currentIndexChanged(int index);

    void on_floorPlanComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
