#include "ampplot.h"

#include <QDebug>

class AmpScale: public QwtScaleDraw
{
public:
    AmpScale()
    {
    }

    virtual QwtText label(double v) const
    {
        return ((new QString())->number(-1*v));
    }
};

AmpPlot::AmpPlot(QwtPlot *plot):
    QwtPlot(parent)
{

//TODO
//    _picker = new QwtPicker(canvas());
//    _picker->setStateMachine(new QwtPickerDragRectMachine());
//    _picker->setTrackerMode(QwtPicker::ActiveOnly);
//    _picker->setRubberBand(QwtPicker::RectRubberBand);
    _plot = plot;

    _panner = new QwtPlotPanner(canvas()); //Panning with the mouse
    _panner->setOrientations(Qt::Horizontal);
    _plot->connect(_panner, SIGNAL(moved(int,int)), this, SLOT(pannerMoved(int,int)));

    _magnifier = new QwtPlotMagnifier(canvas()); //Zooming with the wheel

    plot->canvas()->setBorderRadius(5);
    plot->setCanvasBackground(Qt::white);
    plot->plotLayout()->setAlignCanvasToScales(true);

    //QwtLegend *legend = new QwtLegend;
    //insertLegend(legend,QwtPlot::RightLegend);

    _xMax = DEFAULT_X_MAX;
    _yMax = DEFAULT_Y_MAX;

    plot->setAxisTitle(QwtPlot::xBottom, "ms");
    plot->setAxisScale(QwtPlot::xBottom,0-_xMax,0);
    plot->setAxisScaleDraw(QwtPlot::xBottom, new AmpScale());
    plot->setAxisTitle(QwtPlot::yLeft,"mA");
    plot->setAxisAutoScale(QwtPlot::yLeft, true);

    _curve = new QwtPlotCurve("mA");
    _curve->attach(this);
    _curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    _curve->setVisible(true);

    _time = new QTime();
    _time->start();

    _timeData.clear();
    _data.clear();
}

void AmpPlot::setDataSource(DataSource *source)
{
    qDebug() << "Setting Data Source...";
    if (_dataSource)
        delete _dataSource;
    _dataSource = source;
    connect(_dataSource, SIGNAL(dataRead(double)), this, SLOT(dataRead(double)));
}

void AmpPlot::startRead()
{
    if (_dataSource)
    {
        _data.clear();
        _timeData.clear();
        _dataSource->stopRead();
        _dataSource->startRead();
        _time->restart();
    }
}

void AmpPlot::pauseRead()
{
    if (_dataSource)
        _dataSource->stopRead();
}

void AmpPlot::unpauseRead()
{
    if(_dataSource)
    {
        _time->restart();
        *_time = _time->addMSecs(_timeData.last());
        _dataSource->startRead();

    }
}

QwtPlotCurve *AmpPlot::getCurve()
{
    return _curve;
}

void AmpPlot::dataRead(double value)
{
    double size;
    double lastMean;
    _data.prepend(value);
    _timeData.append(0 - _time->elapsed());

    size = _data.size();
    lastMean = _mean;
    _mean = (1 - (1/size))*_mean + (1/size)*value;
    _sd = ((size-1) * _sd + (value -_mean)*(value - lastMean))* (1 / size);
//    qDebug() << "Mean: " << _mean << "Sd: " << _sd;

    _curve->setRawSamples(_timeData.data(), _data.data(), _data.size());

    emit meanChanged(_mean);

    plot->replot();
}

void AmpPlot::pannerMoved(int dx, int dy)
{
    qDebug() << dx << " - " << dy;
}

AmpPlot::AmpPlot(QwtPlot *plot)
{
}
