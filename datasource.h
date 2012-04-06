#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>

class DataSource : public QObject
{
    Q_OBJECT

public:
    DataSource() {}
    virtual void startRead() {} //Start reading data
    virtual void stopRead() {}
    virtual int getFreq() {return 0;}

signals:
    void dataRead(double value);

};

#endif // DATASOURCE_H
