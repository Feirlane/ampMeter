#include "constantdata.h"

ConstantData::ConstantData()
{

}

void ConstantData::startRead()
{
    _timer = startTimer(50);
}

void ConstantData::stopRead()
{
    killTimer(_timer);
}

int ConstantData::getFreq()
{
    return 50;
}

void ConstantData::timerEvent(QTimerEvent *)
{
    emit dataRead(50.0);
}
