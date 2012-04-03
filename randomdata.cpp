#include "randomdata.h"
#include <QDebug>

RandomData::RandomData()
{

}

void RandomData::startRead()
{
    _timer = startTimer(100);
}

void RandomData::timerEvent(QTimerEvent *)
{
    emit dataRead(qrand() % 100);
}
