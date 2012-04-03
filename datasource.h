#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>

class DataSource : public QObject
{
    Q_OBJECT

public:
    DataSource() {} //Declarar el método vacío aquí es ortodoxo? XD
    virtual void startRead() {} //Start reading data

signals:
    void dataRead(int value);

};

#endif // DATASOURCE_H
