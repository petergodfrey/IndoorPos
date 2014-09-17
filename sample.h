#ifndef SAMPLE_H
#define SAMPLE_H

#include <QString>

#define NOT_SET -1

class Sample {

private:

public:
    QString time;
    int     floorPlanID;
    int     x;
    int     y;
    double  horizontalIntensity;
    double  verticalIntensity;

    Sample(QString line);
    Sample(double _horizontalIntensity, double _verticalIntensity);
    void setCoordinates(int x, int y);
    void setFloorPlanID(int floorPlanID);
};

#endif // SAMPLE_H
