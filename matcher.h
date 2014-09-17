#ifndef MATCHER_H
#define MATCHER_H

#include <QTcpSocket>
#include "databasedriver.h"

class Matcher : public QObject {

    Q_OBJECT

public:
    Matcher(QTcpSocket *socket, DatabaseDriver *db);

private:
    QTcpSocket     *socket;
    DatabaseDriver *db;
    int             floorPlan;
    QTimer          timer;
    QList<Sample>   samples;

public slots:
    void start(int floorPlan);
    void stop();

private slots:
    void process();

signals:
    void locationChanged(const QPoint &location);

};

#endif // MATCHER_H
