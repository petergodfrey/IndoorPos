#include "matcher.h"

Matcher::Matcher(QTcpSocket *_socket, DatabaseDriver *_db) :
    socket       (_socket),
    db           (_db),
    floorPlan    (-1),
    timer        (),
    samples      () {
    connect(&timer, SIGNAL( timeout() ), this, SLOT( process() ) );
}

void Matcher::start(int _floorPlan) {
    floorPlan = _floorPlan;
    timer.start(1000);
}

void Matcher::stop() {
    timer.stop();
}

void Matcher::process() {
    socket->readAll(); // Flush
    samples.clear();   // Flush
    QString line;
    do {
        while ( !socket->canReadLine() ) {
            socket->waitForReadyRead(100);
        }
        line = socket->readLine();
    } while( !line[0].isDigit() );
    Sample s(line);
    s.setFloorPlanID(floorPlan);
    emit locationChanged( db->locate(s) );
}
