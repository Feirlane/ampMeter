#include "ampplot.h"

#include <QDebug>

AmpPlot::AmpPlot(QWidget *parent):
    QwtPlot(parent)
{

    (void)new QwtPlotPanner(canvas()); //Panning with the mouse
    (void)new QwtPlotMagnifier(canvas()); //Zooming with the wheel

    canvas()->setBorderRadius(5);
    plotLayout()->setAlignCanvasToScales(true);

    QwtLegend *legend = new QwtLegend;
    insertLegend(legend,QwtPlot::RightLegend);

    _xMax = DEFAULT_X_MAX;
    _yMax = DEFAULT_Y_MAX;

    setAxisTitle(QwtPlot::xBottom, "ms");
    setAxisScale(QwtPlot::xBottom,0-_xMax,0);
    setAxisTitle(QwtPlot::yLeft,"mA");
    setAxisScale(QwtPlot::yLeft,0,_yMax);

    _curve = new QwtPlotCurve("mA");
    _curve->attach(this);
    _curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    _curve->setVisible(true);

    _time = new QTime();
}

void AmpPlot::setDataSource(DataSource *source)
{
    qDebug() << "Setting Data Source...";
    _dataSource = source;
    connect(_dataSource, SIGNAL(dataRead(int)), this, SLOT(dataRead(int)));
    _dataSource->startRead();
    _time->start();
}

void AmpPlot::dataRead(int value)
{
    double size;
    _data.prepend(value);
    _timeData.append(0 - _time->elapsed());

    size = _data.size();
    _mean = (1 - (1/size))*_mean + (1/size)*value;
    qDebug() << "Mean: " << _mean;

    _curve->setRawSamples(_timeData.data(), _data.data(), _data.size());

    replot();
}
