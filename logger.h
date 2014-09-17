#ifndef LOGGER_H
#define LOGGER_H

#include <QTcpSocket>
#include "databasedriver.h"

class Logger : public QObject {

    Q_OBJECT

public:
    Logger(QTcpSocket *socket, DatabaseDriver *db);
    ~Logger();
    bool isRunning();

private:
    QTcpSocket     *socket;
    DatabaseDriver *db;
    int             floorPlan;
    QPoint          startPoint;
    QPoint          endPoint;
    QList<Sample>   samples;
    bool            mStop;

    void process();

public slots:
    void start(QPoint start, QPoint end, int floorPlan);
    void stop();
    void commit();

};

#endif // LOGGER_H
