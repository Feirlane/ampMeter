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

int RandomData::getFreq()
{
    return 50;
}

void RandomData::timerEvent(QTimerEvent *)
{
    emit dataRead(double(qrand() % 100));
}
