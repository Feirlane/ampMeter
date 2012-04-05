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

AmpPlot::AmpPlot(QwtPlot *plot)
{

//TODO
//    _picker = new QwtPicker(canvas());
//    _picker->setStateMachine(new QwtPickerDragRectMachine());
//    _picker->setTrackerMode(QwtPicker::ActiveOnly);
//    _picker->setRubberBand(QwtPicker::RectRubberBand);
    _plot = plot;

    _panner = new QwtPlotPanner(_plot->canvas()); //Panning with the mouse
    _panner->setOrientations(Qt::Horizontal);
    //connect(_panner, SIGNAL(moved(int,int)), this, SLOT(pannerMoved(int,int)));

    _magnifier = new QwtPlotMagnifier(_plot->canvas()); //Zooming with the wheel

    _plot->canvas()->setBorderRadius(5);
    _plot->setCanvasBackground(Qt::white);
    _plot->plotLayout()->setAlignCanvasToScales(true);

    QwtLegend *legend = new QwtLegend;
    _plot->insertLegend(legend,QwtPlot::RightLegend);
    legend->setItemMode(QwtLegend::CheckableItem);
    connect(_plot,SIGNAL(legendChecked(QwtPlotItem*,bool)),SLOT(showCurve(QwtPlotItem*,bool)));

    _xMax = DEFAULT_X_MAX;
    _yMax = DEFAULT_Y_MAX;

    _plot->setAxisTitle(QwtPlot::xBottom, "ms");
    _plot->setAxisScale(QwtPlot::xBottom,0-_xMax,0);
    _plot->setAxisScaleDraw(QwtPlot::xBottom, new AmpScale());
    _plot->setAxisTitle(QwtPlot::yLeft,"mA");
    _plot->setAxisAutoScale(QwtPlot::yLeft, true);

    _dataCurve = new QwtPlotCurve("mA");
    _dataCurve->attach(_plot);
    _dataCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    _dataCurve->setVisible(_plot);

    _meanCurve = new QwtPlotCurve("avg mean");
    _meanCurve->attach(_plot);
    _meanCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    _meanCurve->setPen(QPen(Qt::red));

    _currentMeanCurve = new QwtPlotCurve("mean");
    _currentMeanCurve->attach(_plot);
    _currentMeanCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    _currentMeanCurve->setPen(QPen(Qt::blue));

    showCurve(_dataCurve, true);
    showCurve(_meanCurve, false);
    showCurve(_currentMeanCurve, true);

    _time = new QTime();
    _time->start();

    _meanData.append(0.0);
    _meanTime.append(0.0);
    _meanData.append(0.0);
    _meanTime.append(0.0);

    _dataSource = NULL;
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
        _dataTime.clear();
        _dataSource->stopRead();
        _dataSource->startRead();
        _meanTime.first() = 0.0;
        _meanTime.last() = 0.0;
        _currentMeanData.clear();
        _currentMeanTime.clear();

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
        *_time = _time->addMSecs(_dataTime.last());
        _dataSource->startRead();

    }
}

QwtPlotCurve *AmpPlot::getCurve()
{
    return _dataCurve;
}

void AmpPlot::dataRead(double value)
{
    double size;
    double lastMean;
    _data.prepend(value);
    _dataTime.append(0 - _dataTime.size()*_dataSource->getFreq());

    size = _data.size();
    lastMean = _mean;
    _mean = (1 - (1/size))*_mean + (1/size)*value;
    _sd = ((size-1) * _sd + (value -_mean)*(value - lastMean))* (1 / size);

    _meanData.first() = _mean;
    _meanData.last() = _mean;
    _meanTime.last() = _dataTime.last();

    _currentMeanData.prepend(_mean);
    _currentMeanTime.append(_dataTime.last());

//    qDebug() << "Mean: " << _mean << "Sd: " << _sd;

    _dataCurve->setRawSamples(_dataTime.data(), _data.data(), _data.size());
    _meanCurve->setRawSamples(_meanTime.data(), _meanData.data(), _meanTime.size());
    _currentMeanCurve->setRawSamples(_currentMeanTime.data(), _currentMeanData.data(), _currentMeanTime.size());

    emit meanChanged(_mean);

    _plot->replot();
}

void AmpPlot::pannerMoved(int dx, int dy)
{
    qDebug() << dx << " - " << dy;
}

void AmpPlot::showCurve(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
    QWidget *w = _plot->legend()->find(item);
    if (w && w->inherits("QwtLegendItem"))
        ((QwtLegendItem *)w)->setChecked(on);

    _plot->replot();
}
