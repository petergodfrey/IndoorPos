#include "databaseexception.h"

DatabaseException::DatabaseException(QObject *parent) : QObject(parent) {
}

DatabaseException::DatabaseException(QString message, QObject *parent) : QObject(parent), mMessage(message) {
}

QString DatabaseException::message() {
    return mMessage;
}
