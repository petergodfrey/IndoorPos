#include <QFile>
#include <QTextStream>
#include <QString>
#include "configurator.h"

Configurator::Configurator() {    
    mHostAddress     = "192.168.0.10";
    mHostPort        = 51600;
    mDatabaseAddress = "localhost";
    mDatabasePort    = 5432;
    mDatabaseName    = "indoor";
}

Configurator::~Configurator() {

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
