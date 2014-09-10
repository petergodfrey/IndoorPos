#ifndef MATCHER_H
#define MATCHER_H

#include <QTcpSocket>
#include "databasedriver.h"

class Matcher : public QObject {
    Q_OBJECT

public:
    Matcher(QTcpSocket *socket, DatabaseDriver *db);
    void setFloorPlanID(int floorPlanID);

private:
    QTcpSocket     *socket;
    DatabaseDriver *db;
    int             floorPlanID;
    QTimer          timer;

public slots:
    void start();
    void stop();
    void process();

signals:
    void locationChanged(const QPoint &location);

};

#endif // MATCHER_H
