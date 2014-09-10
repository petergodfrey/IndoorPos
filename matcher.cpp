#include "matcher.h"

Matcher::Matcher(QTcpSocket *_socket, DatabaseDriver *_db) :
    socket       (_socket),
    db           (_db),
    floorPlanID  (-1),
    timer        () {
    connect(&timer, SIGNAL( timeout() ), this, SLOT( process() ) );
}

void Matcher::setFloorPlanID(int _floorPlanID) {
    floorPlanID = _floorPlanID;
}


void Matcher::start() {
    timer.start(2000);
}

void Matcher::stop() {
    timer.stop();
}

void Matcher::process() {
    //qDebug() << "Matcher::start()";
    socket->readAll(); // Flush
    QString line;
    do {
        while ( !socket->canReadLine() ) {
            socket->waitForReadyRead(100);
        }
        line = socket->readLine();
    } while( !line[0].isDigit() );
    Sample s(line);
    emit locationChanged( db->closestPoint(s, floorPlanID) );

}
