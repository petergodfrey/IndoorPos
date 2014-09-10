#include "sample.h"
#include <cmath>
#include <QStringList>
#include <QDebug>

#define PI 3.14159

Sample::Sample(QString line) {
    QStringList items   = line.split(",");
    time                = items.at(0);
    double ax           = items.at(3).toDouble();
    double ay           = items.at(4).toDouble();
    double az           = items.at(5).toDouble();
    double bx           = items.at(6).toDouble();
    double by           = items.at(7).toDouble();
    double bz           = items.at(8).toDouble();
    double aIntensity   = sqrt( (ax * ax) + (ay * ay) + (az * az) );
    double bIntensity   = sqrt( (bx * bx) + (by * by) + (bz * bz) );
    double theta        = (PI / 2) -
                          acos(
                              ( (ax * bx) + (ay * by) + (az * bz) ) /
                              ( aIntensity * bIntensity )
                              );
    horizontalIntensity = bIntensity * cos(theta);
    verticalIntensity   = bIntensity * sin(theta);
    x                   = NOT_SET;
    y                   = NOT_SET;
    floorPlanID         = NOT_SET;
}

Sample::Sample(double _horizontalIntensity, double _verticalIntensity) :
    horizontalIntensity (_horizontalIntensity),
    verticalIntensity   (_verticalIntensity) {

}

void Sample::setCoordinates(int _x, int _y) {
    x = _x;
    y = _y;
}

void Sample::setFloorPlanID(int _floorPlanID) {
    floorPlanID = _floorPlanID;
}
