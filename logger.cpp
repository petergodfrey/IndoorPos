#include "logger.h"

#include <cmath>
#include <QtCore>
#include <iostream>

Logger::Logger(QObject *parent) :
    QThread   (parent),
    stop      (true),
    r         ("192.168.0.10", 51600),
    xStart    (0),
    yStart    (0),
    xEnd      (0),
    yEnd      (0),
    startTime (0),
    endTime   (0) {
}

void Logger::run() {
    // Read samples
    startTime = time(0);
    while (true) {
        QMutex mutex;
        mutex.lock();
        if (this->stop) break;
        mutex.unlock();
        samples.push_back( new Sample( r.readLine() ) );
    }
    endTime = time(0);

    // Euclidian distance between start & end coordinates
    double distance          = sqrt( ((xStart - xEnd) * (xStart - xEnd)) +
                                     ((yStart - yEnd) * (yStart - yEnd)) );
    // The distance travelled between each sample
    double distancePerSample = distance / (samples.size() - 1);
    // The angle of travel relative to the plane of the map
    double theta             = atan( fabs(yStart - yEnd) / fabs(xStart - xEnd) );
    int n = 0;
    for (std::vector<Sample *>::iterator it = samples.begin(); it != samples.end(); it++) {
        double x = xStart + ( n * distancePerSample * cos(theta) );
        double y = yStart + ( n * distancePerSample * sin(theta) );
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


/*
std::vector<Sample *> Logger::begin(int xStart, int yStart, int xEnd, int yEnd) {
    time_t startTime = time(0);
    std::vector<Sample *> samples;
    while (true) {
         QApplication::processEvents();
        if (stopRequested) break;
        samples.push_back( new Sample( r.readLine() ) );
    }
    time_t endTime           = time(0);
    double distance          = sqrt( ((xStart - xEnd) * (xStart - xEnd)) +
                                     ((yStart - yEnd) * (yStart - yEnd)) );
    double distancePerSample = distance / (samples.size() - 1);
    double theta             = atan( fabs(yStart - yEnd) / fabs(xStart - xEnd) );
    int n = 0;
    for (std::vector<Sample *>::iterator it = samples.begin(); it != samples.end(); it++) {
        double x = xStart + ( n * distancePerSample * cos(theta) );
        double y = yStart + ( n * distancePerSample * sin(theta) );
        x = round(x);
        y = round(y);
        //if ( pointExists(x, y) ) {
            //update with average
        //}
        n++;
    }
}
*/
