#include "constantdata.h"

ConstantData::ConstantData()
{
    _time.start();
}

void ConstantData::startRead()
{
    _timer = startTimer(50);
    _time.restart();
}

void ConstantData::stopRead()
{
    killTimer(_timer);
}

void ConstantData::timerEvent(QTimerEvent *)
{
    emit dataRead(50.0, _time.elapsed());
}
