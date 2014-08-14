#ifndef LOGGER_H
#define LOGGER_H

#include <QThread>
#include "sample.h"
#include "reader.h"

class Logger : public QThread {

    Q_OBJECT

public:
    explicit Logger(QObject *parent = 0);
    //void begin(int xStart, int yStart, int xEnd, int yEnd);
    void run();
    bool stop;
    int                   xStart;
    int                   yStart;
    int                   xEnd;
    int                   yEnd;

private:
    Reader                r;
    std::vector<Sample *> samples;
    time_t                startTime;
    time_t                endTime;

signals:


public slots:

};

#endif // LOGGER_H
