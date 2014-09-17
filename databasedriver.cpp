#include "databasedriver.h"
#include "Exceptions/databaseexception.h"

#define BUILDINGS_MODEL_QUERY  "SELECT name, id FROM Buildings  ORDER BY name;"
#define FLOORPLANS_MODEL_QUERY "SELECT name, id FROM FloorPlans ORDER BY name;"

DatabaseDriver::DatabaseDriver(QString address, int port, QString name, QObject *parent) : QObject(parent) {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(address);
    db.setPort(port);
    db.setDatabaseName(name);
    db.open();
    mBuildingsModel             = new QSqlTableModel(this, db);
    mLoggingFloorPlansModel     = new QSqlTableModel(this, db);
    mPositioningFloorPlansModel = new QSqlTableModel(this, db);
}

DatabaseDriver::~DatabaseDriver() {
    db.close();
}

bool DatabaseDriver::isConnected() {
    return db.isOpen();
}

QSqlQueryModel* DatabaseDriver::buildingsModel() {
    mBuildingsModel->setQuery(BUILDINGS_MODEL_QUERY);
    return mBuildingsModel;
}

QSqlQueryModel* DatabaseDriver::loggingFloorPlansModel(int index) {
    int id = mBuildingsModel->record(index).value("id").toInt();
    QString t;
    t = QString(
        "SELECT   name, id "
        "FROM     Floorplans "
        "WHERE    building = %1 "
        "ORDER BY name;"
        ).arg(id);
    mLoggingFloorPlansModel->setQuery(t);
    return mLoggingFloorPlansModel;
}

QSqlQueryModel* DatabaseDriver::positioningFloorPlansModel(int index) {
    int id = mBuildingsModel->record(index).value("id").toInt();
    QString t;
    t = QString(
        "SELECT   name, id "
        "FROM     Floorplans "
        "WHERE    building = %1 "
        "ORDER BY name;"
        ).arg(id);
    mPositioningFloorPlansModel->setQuery(t);
    return mPositioningFloorPlansModel;
}

int DatabaseDriver::buildingID(int index) {
    return mBuildingsModel->record(index).value("id").toInt();
}

int DatabaseDriver::loggingFloorPlanID(int index) {
    return mLoggingFloorPlansModel->record(index).value("id").toInt();
}

int DatabaseDriver::positioningFloorPlanID(int index) {
    return mPositioningFloorPlansModel->record(index).value("id").toInt();
}

QString DatabaseDriver::floorPlanImagePath(int floorPlan) {
    QSqlQuery q;
    QString   t;
    t = QString(
        "SELECT map "
        "FROM   Floorplans "
        "WHERE  id = %1;"
        ).arg(floorPlan);
    q.exec(t);
    q.next();
    return q.value(0).toString();
}

int DatabaseDriver::floorPlanWidth(int floorPlan) {
    QSqlQuery q;
    QString   t;
    t = QString(
        "SELECT width "
        "FROM   Floorplans "
        "WHERE  id = %1;"
        ).arg(floorPlan);
    q.exec(t);
    q.next();
    return q.value(0).toInt();
}

void DatabaseDriver::addSample(Sample s) {
    QSqlQuery q;
    QString   t;
    t = QString(
        "SELECT horizontalIntensity, verticalIntensity "
        "FROM   Samples "
        "WHERE  floorPlan = %1 AND x = %2 AND y = %3;"
        ).arg(s.floorPlanID).arg(s.x).arg(s.y);
    q.exec(t);
    if ( q.next() ) { // Record already exists
        s.horizontalIntensity = (q.value(0).toDouble() + s.horizontalIntensity) / 2; // Take average
        s.verticalIntensity   = (q.value(1).toDouble() + s.verticalIntensity)   / 2;
        t = QString(
            "UPDATE Samples "
            "SET    horizontalIntensity = %1, verticalIntensity = %2 "
            "WHERE  floorPlan = %3 AND x = %4 AND y = %5;"
            ).arg(s.floorPlanID).arg(s.horizontalIntensity).arg(s.verticalIntensity).arg(s.x).arg(s.y);
    } else {          // Record does not already exist
        t = QString(
            "INSERT INTO Samples (floorPlan, x, y, verticalIntensity, horizontalIntensity) "
            "VALUES (%1, %2, %3, %4, %5);"
             ).arg(s.floorPlanID).arg(s.x).arg(s.y).arg(s.verticalIntensity).arg(s.horizontalIntensity);
    }
    if ( q.exec(t) ) {
        qDebug() << "addSample Success";
    } else {
        qDebug() << "ERROR: DatabaseDriver::addSample " << db.lastError().text();
    }
}

void DatabaseDriver::addBuilding(QString name, QString address) {
    QSqlQuery q;
    QString   t;
    t = QString(
        "INSERT INTO Buildings (name, address) "
        "VALUES ('%1', '%2');"
        ).arg(name).arg(address);
    if ( q.exec(t) ) {
        qDebug() << "addBuilding Success";
    } else {
        qDebug() << "ERROR: DatabaseDriver::addBuilding " << db.lastError().text();
    }
    mBuildingsModel->setQuery(BUILDINGS_MODEL_QUERY);
}

void DatabaseDriver::addFloorplan(int building, QString name, QString level, QString map, int width) {
    QSqlQuery q;
    QString   t;
    t = QString(
        "INSERT INTO FloorPlans (building, name, level, map, width) "
        "VALUES (%1, '%2', '%3', '%4', %5);"
        ).arg(building).arg(name).arg(level).arg(map).arg(width);
    if ( q.exec(t) ) {
        qDebug() << "addFloorplan Success";
    } else {
        qDebug() << "ERROR: DatabaseDriver::addFloorplan " << t;//db.lastError().text();
    }
    mLoggingFloorPlansModel->setQuery(FLOORPLANS_MODEL_QUERY);
}

void DatabaseDriver::deleteFloorPlan(int floorPlan) {
    QDir d;
    d.remove( floorPlanImagePath(floorPlan) );
    QSqlQuery q;
    QString   t;
    t = QString(
        "DELETE FROM Samples "
        "WHERE       floorplan = %1;"
        ).arg(floorPlan);
    q.exec(t);
    t = QString(
        "DELETE FROM FloorPlans "
        "WHERE       id = %1;"
        ).arg(floorPlan);
    q.exec(t);
    mLoggingFloorPlansModel->setQuery(FLOORPLANS_MODEL_QUERY);
}



QPoint DatabaseDriver::closestPoint(Sample sample, int floorPlan) {
    QSqlQuery q;
    QString   t;
    t = QString(
        "SELECT x, y, verticalIntensity, horizontalIntensity "
        "FROM   Samples "
        "WHERE  floorplan = %1;"
        ).arg(floorPlan);
    q.exec(t);

    int x = 0;
    int y = 0;
    double minDifference = 99999999;
    while ( q.next() ) {
        double horizontalDifference = fabs( sample.horizontalIntensity - q.value(3).toDouble() );
        double verticalDifference   = fabs( sample.verticalIntensity   - q.value(2).toDouble() );
        if ( (horizontalDifference + verticalDifference) < minDifference) {
            minDifference = horizontalDifference + verticalDifference;
            x = q.value(0).toInt();
            y = q.value(1).toInt();
        }
    }
    return QPoint(x, y);
}

// Choose the M closest locations and return the average (centroid location)
QPoint DatabaseDriver::locate(Sample sample) {
    QSqlQuery q;
    QString   t;
    t = QString(
        "SELECT   x, y, ABS(%1 - verticalIntensity) + ABS(%2 - horizontalIntensity) AS difference"
        "FROM     Samples "
        "WHERE    floorplan = %3 "
        "ORDER BY difference;"
        ).arg(sample.verticalIntensity
        ).arg(sample.horizontalIntensity
        ).arg(sample.floorPlanID);
    q.exec(t);
    int xSum = 0;
    int ySum = 0;
    int n;
    for (n = 0; n < 3 && q.next(); n++) {
         xSum += q.value(0).toInt();
         ySum += q.value(1).toInt();
    }
    return QPoint(xSum / n, ySum / n);
}
