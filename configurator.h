#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QString>

class Configurator {

public:
    Configurator();
    QString hostAddress();
    int     hostPort();
    QString databaseAddress();
    int     databasePort();
    QString databaseName();

private:
    QString mHostAddress;
    int     mHostPort;
    QString mDatabaseAddress;
    int     mDatabasePort;
    QString mDatabaseName;

};

#endif // CONFIGURATOR_H
