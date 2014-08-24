#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QString>

class Configurator {

public:
    Configurator();
    ~Configurator();
    QString hostAddress(void);
    int     hostPort(void);
    QString databaseAddress(void);
    int     databasePort(void);
    QString databaseName(void);

private:
    QString mHostAddress;
    int     mHostPort;
    QString mDatabaseAddress;
    int     mDatabasePort;
    QString mDatabaseName;

};

#endif // CONFIGURATOR_H
