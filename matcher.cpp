#include "matcher.h"

Matcher::Matcher(QTcpSocket *_socket, DatabaseDriver *_db) :
    socket       (_socket),
    db           (_db),
    floorPlan    (-1) {
    connect(&timer, SIGNAL( timeout() ), this, SLOT( process() ) );
}

void Matcher::start(int _floorPlan) {
    floorPlan = _floorPlan;
    timer.start(2000);
}

void Matcher::stop() {
    timer.stop();
}

void Matcher::process() {
    qDebug() << "Matcher::process()";

    socket->readAll(); // Flush
    samples.clear();   // Flush
    QString line;
    double v = 0;
    double h = 0;
    for (int i = 0; i < 10; i++) {
        do {
            do {
                socket->waitForReadyRead(100);
            } while ( !socket->canReadLine() );
            line = socket->readLine();
        } while( !line[0].isDigit() );
        Sample s(line);
        v += s.verticalIntensity;
        h += s.horizontalIntensity;
    }
    v = v / 10;
    h = h / 10;
    Sample t(h, v);
    t.setFloorPlanID(floorPlan);
    emit locationChanged( db->locateClosestM(t) );
}
