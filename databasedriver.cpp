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
    QString   s;
    s = QString("select name, id from Buildings;");
    q.exec(s);
    QVector< QPair<QString, int> > v;
    while ( q.next() ) {
        v.append( QPair<QString, int>(q.value(0).toString(), q.value(1).toInt() ) );
    }
    return v;
}

QStringList DatabaseDriver::getFloorplanNames(int building) {
    QSqlQuery q;
    QString   s;
    s = QString("select name from Floorplans where building = %1;").arg(building);
    q.exec(s);
    QStringList l;
    while ( q.next() ) {
        l.append( q.value(0).toString() );
    }
    return l;
}


void DatabaseDriver::addSample(Sample s) {
    QSqlQuery q;
    QString   t;
    t = QString("select horizontalIntensity, verticalIntensity from Samples where floorPlan = %1 AND x = %2 AND y = %3;").arg(s.floorPlanID).arg(s.x).arg(s.y);
    q.exec(t);
    if ( q.next() ) { // Record already exists
        s.horizontalIntensity = (q.value(0).toDouble() + s.horizontalIntensity) / 2; // Take average
        s.verticalIntensity   = (q.value(1).toDouble() + s.verticalIntensity)   / 2;
        t = QString("update Samples set horizontalIntensity = %1, verticalIntensity = %2 where floorPlan = %3 AND x = %4 AND y = %5;").arg(s.floorPlanID).arg(s.horizontalIntensity).arg(s.verticalIntensity).arg(s.x).arg(s.y);
    } else {          // Record does not already exist
        t = QString("insert into Samples (floorPlan, x, y, verticalIntensity, horizontalIntensity) values (%1, %2, %3, %4, %5);").arg(s.floorPlanID).arg(s.x).arg(s.y).arg(s.verticalIntensity).arg(s.horizontalIntensity);

    }
    if ( q.exec(t) ) {
        qDebug() << "addSample Success";
    } else {
        qDebug() << "addSample ERROR";
    }
}
