#include "databasedriver.h"

DatabaseDriver::DatabaseDriver(QString address, int port, QString name, QObject *parent) : QObject(parent) {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(address);
    db.setPort(port);
    db.setDatabaseName(name);
    if ( db.open() ) {
        qDebug() << "Database Opened";
    } else {
        qDebug() << "DatabaseDriver() ERROR: " << db.lastError().text();
    }
}

DatabaseDriver::~DatabaseDriver() {
    db.close();
}

QVector< QPair<QString, int> >DatabaseDriver::getBuildings(void) {
    QSqlQuery q;
    QString   t;
    t = QString(
        "SELECT name, id "
        "FROM   Buildings "
        "ORDER BY name;"
        );
    q.exec(t);
    QVector< QPair<QString, int> > v;
    while ( q.next() ) {
        v.append( QPair<QString, int>( q.value(0).toString(), q.value(1).toInt() ) );
    }
    return v;
}

QVector< QPair<QString, int> > DatabaseDriver::getFloorplanNames(int building) {
    QSqlQuery q;
    QString   t;
    t = QString(
        "SELECT name, id "
        "FROM   Floorplans "
        "WHERE  building = %1 "
        "ORDER BY name;"
        ).arg(building);
    q.exec(t);
    QVector< QPair<QString, int> > v;
    while ( q.next() ) {
        v.append( QPair<QString, int>( q.value(0).toString(), q.value(1).toInt() ) );
    }
    return v;
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
}

void DatabaseDriver::addFloorplan(int building, QString name, QString level, QString map) {
    QSqlQuery q;
    QString   t;
    t = QString(
        "INSERT INTO FloorPlans (building, name, level, map) "
        "VALUES (%1, '%2', '%3', '%4');"
        ).arg(building).arg(name).arg(level).arg(map);
    if ( q.exec(t) ) {
        qDebug() << "addFloorplan Success";
    } else {
        qDebug() << "ERROR: DatabaseDriver::addFloorplan " << db.lastError().text();
    }
}
