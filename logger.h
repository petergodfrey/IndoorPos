#ifndef LOGGER_H
#define LOGGER_H

#include <QThread>
#include <QTcpSocket>
#include "databasedriver.h"

class Logger : public QThread {

    Q_OBJECT

public:
    explicit Logger(QTcpSocket *s, DatabaseDriver *_db, QObject *parent = 0);
    ~Logger();
    void run();
    void commit();
    void setFloorPlanID(int floorPlanID);
    void setStartPoint(QPoint p);
    void setEndPoint(QPoint p);
    bool stop;

private:
    QTcpSocket     *socket;
    DatabaseDriver *db;
    int             floorPlanID;
    QString         databaseName;
    QString         databaseServerName;
    int             databasePort;
    QPoint          startPoint;
    QPoint          endPoint;
    QList<Sample *> samples;

signals:

public slots:

};

#endif // LOGGER_H
