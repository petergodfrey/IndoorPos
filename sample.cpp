#include "sample.h"

#include <QDebug>

#include <cmath>
#include <QStringList>

#define PI 3.14159

Sample::Sample(QString line) {
    QStringList items   = line.split(",");
    time                = items.at(0);
    timestamp           = items.at(1);
    recordTime          = items.at(2);
    double ax           = items.at(3).toDouble();
    double ay           = items.at(4).toDouble();
    double az           = items.at(5).toDouble();
    double bx           = items.at(6).toDouble();
    double by           = items.at(7).toDouble();
    double bz           = items.at(8).toDouble();
    double aIntensity   = (ax * ax) + (ay * ay) + (az * az);
    double bIntensity   = (bx * bx) + (by * by) + (bz * bz);
    double theta        = (PI / 2) - acos(aIntensity /
                          ( sqrt(aIntensity) * sqrt(bIntensity) ) );
    horizontalIntensity = bIntensity * cos(theta);
    verticalIntensity   = bIntensity * sin(theta);
}
