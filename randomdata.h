#ifndef RANDOMDATA_H
#define RANDOMDATA_H

#include <QObject>

#include "datasource.h"

class RandomData : public DataSource
{
    Q_OBJECT
public:
    RandomData();
    void startRead();
    void stopRead();

protected:
    void timerEvent(QTimerEvent *e);

private:
    int _timer;

 };

#endif // RANDOMDATA_H
