#ifndef LOGGER_H
#define LOGGER_H

#include <QThread>
#include <QTcpSocket>

class Logger : public QThread {

    Q_OBJECT

public:
    explicit Logger(QObject *parent = 0);
    void run();
    bool stop;
    int  xStart;
    int  yStart;
    int  xEnd;
    int  yEnd;

private:
    QTcpSocket *socket;

signals:

public slots:

};

#endif // LOGGER_H
