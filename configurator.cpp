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

QString Configurator::hostAddress(void) {
    return mHostAddress;
}

int Configurator::hostPort(void) {
    return mHostPort;
}

QString Configurator::databaseAddress(void) {
    return mDatabaseAddress;
}

int Configurator::databasePort(void) {
    return mDatabasePort;
}

QString Configurator::databaseName(void) {
    return mDatabaseName;
}
