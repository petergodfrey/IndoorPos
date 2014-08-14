#include "socket.h"

Socket::Socket(QObject *parent) :
    QObject(parent)
{
}

void Socket::connect() {
    socket = new QTcpSocket(this);
    socket->connectToHost("192.168.0.10", 51600);
    if ( socket->waitForConnected(3000) ) {

    }
}
