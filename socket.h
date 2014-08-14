#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>

class Socket : public QObject {

    Q_OBJECT

public:
    explicit Socket(QObject *parent = 0);
    void connect();

private:
    QTcpSocket *socket;

signals:

public slots:

};

#endif // SOCKET_H
