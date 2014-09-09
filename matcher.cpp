#include "matcher.h"

Matcher::Matcher(QTcpSocket *s, DatabaseDriver *_db, QObject *parent) :
    QThread      (parent),
    stop         (true),
    socket       (s),
    db           (_db),
    floorPlanID  (-1) {
}

void Matcher::run() {
    socket->readAll(); // Flush
    while (true) {
        QMutex mutex;
        mutex.lock();
        if (this->stop) return;//break;
        mutex.unlock();

        qDebug() << "run()";
        double hSum = 0;
        double vSum = 0;
        for (int i = 0; i < 10; i++) {
            QString line;
            do {
                while ( !socket->canReadLine() ) {
                    socket->waitForReadyRead(100);
                }
                line = socket->readLine();
            } while( !line[0].isDigit() );
            Sample s(line);
            hSum += s.horizontalIntensity;
            vSum += s.verticalIntensity;
        }

        Sample t(hSum / 10, vSum / 10);
        mutex.lock();
        QPoint p( db->closestPoint(t, floorPlanID) );
        location = p;
        mutex.unlock();
    }
}
