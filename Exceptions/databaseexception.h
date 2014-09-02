#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <QObject>

class DatabaseException : public QObject {

    Q_OBJECT

public:
    explicit DatabaseException(QObject *parent = 0);
    DatabaseException(QString message, QObject *parent = 0);
    QString message();

private:
    QString mMessage;

signals:

public slots:

};

#endif // DATABASEEXCEPTION_H
