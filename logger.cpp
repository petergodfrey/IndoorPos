#include "logger.h"

#include <QDebug>

#include "sample.h"
#include <QList>
#include <cmath>

#define MIN_LINE_LENGTH 106

Logger::Logger(QObject *parent) :
    QThread (parent),
    stop    (true),
    xStart  (0),
    yStart  (0),
    xEnd    (0),
    yEnd    (0) {
    socket = new QTcpSocket(this);
    socket->connectToHost("192.168.0.10", 51600);
    if ( socket->waitForConnected(1000) ) {
        qDebug() << "Connected";
    } else {
        qDebug() << "Connection Failed";
    }
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
    // Euclidian distance between start & end coordinates
    double distance           = sqrt( ((xStart - xEnd) * (xStart - xEnd)) +
                                      ((yStart - yEnd) * (yStart - yEnd)) );
    // The "as the crow flies" distance travelled between each sample
    double distancePerSample  = distance / (samples.size() - 1);
    // The angle of travel relative to the plane of the map
    double theta              = atan( fabs(yStart - yEnd) / fabs(xStart - xEnd) );
    // The x & y components of the distance travelled between each sample
    double xDistancePerSample = distancePerSample * cos(theta);
    double yDistancePerSample = distancePerSample * sin(theta);
    unsigned int n = 0;
    for (QList<Sample *>::iterator it = samples.begin(); it != samples.end(); it++) {
        double x = xStart + (n * xDistancePerSample);
        double y = yStart + (n * yDistancePerSample);
        x = round(x);
        y = round(y);
        // Here is where we connect to the database
        //if data for the point (x, y) already exists
              //update with average
        //else
              // insert new entry
        n++;
    }
}
