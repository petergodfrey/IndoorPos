#ifndef MATCHER_H
#define MATCHER_H

#include <QThread>

class Matcher : public QThread
{
    Q_OBJECT
public:
    explicit Matcher(QObject *parent = 0);

signals:

public slots:

};

#endif // MATCHER_H
