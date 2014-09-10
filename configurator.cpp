#include <QFile>
#include <QDir>

#include "configurator.h"

Configurator::Configurator() :
    mHostAddress     ("192.168.0.11"),
    mHostPort        (51600),
    mDatabaseAddress ("localhost"),
    mDatabasePort    (5432),
    mDatabaseName    ("indoor") {

    /*
    QFile file( QDir::currentPath().append("/geo.conf") );

    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        //error
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    while ( !line.isNull() ) {

    }
    */
}

QString Configurator::hostAddress() {
    return mHostAddress;
}

int Configurator::hostPort() {
    return mHostPort;
}

QString Configurator::databaseAddress() {
    return mDatabaseAddress;
}

int Configurator::databasePort() {
    return mDatabasePort;
}

QString Configurator::databaseName() {
    return mDatabaseName;
}
