#ifndef MATCHER_H
#define MATCHER_H

#include <QThread>
#include <QTcpSocket>

#include "databasedriver.h"

#define BUFFER_SIZE 100

class Matcher : public QThread {

    Q_OBJECT

public:
    explicit Matcher(QTcpSocket *s, DatabaseDriver *_db, QObject *parent = 0);
    void run();
    void setFloorPlanID(int _floorPlanID);
    bool            stop;
    QPoint          location;

private:
    QTcpSocket     *socket;
    DatabaseDriver *db;
    int             floorPlanID;


signals:
    void locationChanged();

public slots:

};

#endif // MATCHER_H
