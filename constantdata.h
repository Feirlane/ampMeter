#ifndef CONSTANTDATA_H
#define CONSTANTDATA_H

#include <QObject>

#include "datasource.h"

class ConstantData : public DataSource
{
    Q_OBJECT
public:
    ConstantData();
    void startRead();
    void stopRead();

protected:
    void timerEvent(QTimerEvent *e);

private:
    int _timer;

};

#endif // CONSTANTDATA_H
