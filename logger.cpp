#include "logger.h"

#define MIN_LINE_LENGTH 106

Logger::Logger(QTcpSocket *_socket, DatabaseDriver *_db) :
    socket       (_socket),
    db           (_db),
    floorPlan    (-1),
    startPoint   (0, 0),
    endPoint     (0, 0),
    mStop        (true) {
}

Logger::~Logger() {

}

bool Logger::isRunning() {
    return !mStop;
}

void Logger::process() {
    samples.clear();
    socket->readAll(); // Flush
    while (mStop == false) {
        QString line;
        do {
            do {
                socket->waitForReadyRead(100);
            } while ( !socket->canReadLine() );
            line = socket->readLine();
        } while( !line[0].isDigit() );
        qDebug() << line;
        samples.append( Sample(line) );
    }
}

void Logger::start(QPoint start, QPoint end, int _floorPlan) {
    startPoint = start;
    endPoint   = end;
    floorPlan = _floorPlan;
    mStop = false;
    process();
}

void Logger::stop() {
    mStop = true;
}

void Logger::commit() {
    double distance           = sqrt( ( (startPoint.x() - endPoint.x()) * (startPoint.x() - endPoint.x()) ) +
                                      ( (startPoint.y() - endPoint.y()) * (startPoint.y() - endPoint.y()) ) );
    double distancePerSample  = distance / (samples.size() - 1);
    double theta              = atan( fabs( startPoint.y() - endPoint.y() ) / fabs( startPoint.x() - endPoint.x() ) );
    double xDistancePerSample = distancePerSample * cos(theta);
    double yDistancePerSample = distancePerSample * sin(theta);
    unsigned int n = 0;
    for (QList<Sample>::iterator it = samples.begin(); it != samples.end(); it++) {
        double xd = startPoint.x() + (n * xDistancePerSample);
        if ( startPoint.x() > endPoint.x() ) {
            xd = startPoint.x() - (n * xDistancePerSample);
        }
        double yd = startPoint.y() + (n * yDistancePerSample);
        if ( startPoint.y() > endPoint.y() ) {
            yd = startPoint.y() - (n * yDistancePerSample);
        }
        int x = round(xd);
        int y = round(yd);
        qDebug() << "(" << x << "," << y << ")";
        it->setCoordinates(x, y);
        it->setFloorPlanID(floorPlan);
        db->addSample(*it);
        n++;
    }
}
