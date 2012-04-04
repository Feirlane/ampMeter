#include "randomdata.h"

RandomData::RandomData()
{

}

void RandomData::startRead()
{
    _timer = startTimer(50);
}

void RandomData::stopRead()
{
    killTimer(_timer);
}

void RandomData::timerEvent(QTimerEvent *)
{
    emit dataRead(double(qrand() % 100));
}
