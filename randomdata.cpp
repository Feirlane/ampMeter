#include "randomdata.h"

RandomData::RandomData()
{
    _time.start();
}

void RandomData::startRead()
{
    _timer = startTimer(50);
    _time.restart();
}

void RandomData::stopRead()
{
    killTimer(_timer);
}

void RandomData::timerEvent(QTimerEvent *)
{
    emit dataRead(double(qrand() % 100), _time.elapsed());
}
