#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QTime>

class DataSource : public QObject
{
    Q_OBJECT

public:
    DataSource() {}
    virtual void startRead() {} //Start reading data
    virtual void stopRead() {}

signals:
    void dataRead(double sample, double time);

protected:
    QTime _time;

};

#endif // DATASOURCE_H
