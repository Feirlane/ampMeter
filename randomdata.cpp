#include "randomdata.h"
#include <QDebug>

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
    emit dataRead(qrand() % 100);
}
