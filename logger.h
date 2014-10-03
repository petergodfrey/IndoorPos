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

public slots:
    void start(QPoint start, QPoint end, int floorPlan);
    void stop();
    void commit();

private slots:
    void process();
};

#endif // LOGGER_H
