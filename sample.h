#ifndef SAMPLE_H
#define SAMPLE_H

#include <QString>

class Sample {

private:
    QString time;
    QString timestamp;
    QString recordTime;
    int     horizontalIntensity;
    int     verticalIntensity;

public:
    Sample(QString line);

};

#endif // SAMPLE_H
