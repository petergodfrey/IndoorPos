#ifndef DATABASEDRIVER_H
#define DATABASEDRIVER_H

#include "sample.h"
#include <QObject>
#include <QtSql>
#include <QVector>
#include <QPair>
#include <QString>
#include <QStringList>

class DatabaseDriver : public QObject {

    Q_OBJECT

public:
    explicit DatabaseDriver(QString address, int port, QString name, QObject *parent = 0);
    ~DatabaseDriver(void);
    void addSample(Sample sample);
    QVector< QPair<QString, int> > getBuildings(void);
    QStringList getFloorplanNames(int building);
private:
    QSqlDatabase db;

signals:

public slots:

};

#endif // DATABASEDRIVER_H
