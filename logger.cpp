#include "logger.h"
#include "sample.h"
#include <QList>
#include <cmath>

#define MIN_LINE_LENGTH 106

Logger::Logger(QString address, int port, DatabaseDriver *_db, QObject *parent) :
    QThread      (parent),
    stop         (true),
    db           (_db),
    floorPlanID  (-1) {

    socket = new QTcpSocket(this);
    socket->connectToHost(address, port);
    if ( socket->waitForConnected(1000) ) {
        qDebug() << "Connected to host";
    } else {
        qDebug() << "Connection to host failed";
    }
}

Logger::~Logger() {
    delete socket;
}

void Logger::setFloorPlanID(int _floorPlanID) {
    floorPlanID = _floorPlanID;
}

void Logger::setStartPoint(QPoint p) {
    startPoint.setX( p.x() );
    startPoint.setY( p.y() );
}

void Logger::setEndPoint(QPoint p) {
    endPoint.setX( p.x() );
    endPoint.setY( p.y() );
}

void Logger::run() {
    QList<Sample *> samples;
    socket->readAll(); // Flush
    while (true) {
        QMutex mutex;
        mutex.lock();
        if (this->stop) break;
        mutex.unlock();
        QString line;
        do {
            while ( !socket->canReadLine() ) {
                socket->waitForReadyRead(100);
            }
            line = socket->readLine();
        } while( !line[0].isDigit() );
        qDebug() << line;
        samples.append( new Sample(line) );
    }
    double distance           = sqrt( ( (startPoint.x() - endPoint.x()) * (startPoint.x() - endPoint.x()) ) +
                                      ( (startPoint.y() - endPoint.y()) * (startPoint.y() - endPoint.y()) ) );
    double distancePerSample  = distance / (samples.size() - 1);
    double theta              = atan( fabs( startPoint.y() - endPoint.y() ) / fabs( startPoint.x() - endPoint.x() ) );
    double xDistancePerSample = distancePerSample * cos(theta);
    double yDistancePerSample = distancePerSample * sin(theta);
    unsigned int n = 0;
    for (QList<Sample *>::iterator it = samples.begin(); it != samples.end(); it++) {
        double xd = startPoint.x() + (n * xDistancePerSample);
        double yd = startPoint.y() + (n * yDistancePerSample);
        int x = round(xd);
        int y = round(yd);
        (*it)->setCoordinates(x, y);
        (*it)->setFloorPlanID(floorPlanID);
        db->addSample(**it);
        n++;
    }
}
