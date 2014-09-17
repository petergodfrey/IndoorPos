#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "databasedriver.h"
#include "logger.h"
#include "configurator.h"
#include "matcher.h"
#include "imageviewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Configurator   *c;
    DatabaseDriver *db;
    QTcpSocket     *s;
    Logger         *l;
    Matcher        *m;
    QThread        loggingThread;
    QThread        positioningThread;
    QTimer         *timer;
    ImageViewer    *loggingImageViewer;
    ImageViewer    *positioningImageViewer;

signals:
    void startLogging(QPoint start, QPoint end, int floorPlan);
    void stopLogging();
    void startPositioning(int floorPlan);
    void stopPositioning();

private slots:
     void on_tabWidget_currentChanged(int index);

    // Logging Tab
    void on_loggingBuildingComboBox_currentIndexChanged(int index);
    void on_loggingFloorPlanComboBox_currentIndexChanged(int index);
    void on_newBuildingPushButton_clicked();
    void on_newFloorPlanPushButton_clicked();
    void on_buildingDeletePushButton_clicked();
    void on_floorPlanDeletePushButton_clicked();
    void on_loggingStartButton_clicked();
    void on_loggingStopButton_clicked() ;
    void on_loggingZoomInPushButton_clicked();
    void on_loggingZoomOutPushButton_clicked();

    // Positioning Tab
    void on_positioningBuildingComboBox_currentIndexChanged(int index);
    void on_positioningFloorPlanComboBox_currentIndexChanged(int index);
    void on_positioningZoomOutPushButton_clicked();
    void on_positioningZoomInPushButton_clicked();
    void on_positioningStartButton_clicked();
    void on_positioningStopButton_clicked();
    void positioningLocationChanged(const QPoint &location);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
