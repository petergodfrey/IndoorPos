#ifndef LOGGER_H
#define LOGGER_H

#include <QThread>
#include <QTcpSocket>
#include "databasedriver.h"

class Logger : public QThread {

    Q_OBJECT

public:
    explicit Logger(QString address, int port, DatabaseDriver *_db, QObject *parent = 0);
    ~Logger();
    void run();
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

signals:

public slots:

};

#endif // LOGGER_H
