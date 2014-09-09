#ifndef DATABASEDRIVER_H
#define DATABASEDRIVER_H

#include "sample.h"
#include <QObject>
#include <QtSql>
#include <QVector>
#include <QPair>
#include <QString>

class DatabaseDriver : public QObject {

    Q_OBJECT

public:
    explicit DatabaseDriver(QString address, int port, QString name, QObject *parent = 0);
    ~DatabaseDriver();
    QSqlQueryModel* buildingsModel();
    QSqlQueryModel* loggingFloorPlansModel(int index);
    QSqlQueryModel* positioningFloorPlansModel(int index);
    int             buildingID(int index);
    int             loggingFloorPlanID(int index);
    int             positioningFloorPlanID(int index);
    QString         floorPlanImagePath(int floorPlan);
    void            addSample(Sample sample);
    void            addBuilding(QString name, QString address);
    void            addFloorplan(int building, QString name, QString level, QString map);
    void            deleteFloorPlan(int floorPlan);

    QPoint          closestPoint(Sample sample, int floorPlan);

private:
    QSqlDatabase    db;
    QSqlQueryModel *mBuildingsModel;
    QSqlQueryModel *mLoggingFloorPlansModel;
    QSqlQueryModel *mPositioningFloorPlansModel;

};

#endif // DATABASEDRIVER_H
