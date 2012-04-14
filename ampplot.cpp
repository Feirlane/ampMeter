#include "ampplot.h"

#include <QDebug>

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
    _plot->setAxisScale(QwtPlot::xBottom,0,_xMax);

    QwtScaleWidget *scaleWidget = _plot->axisWidget(QwtPlot::xBottom);
    const int fmh = QFontMetrics(scaleWidget->font()).height();
    scaleWidget->setMinBorderDist(0, fmh*2);

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

    _pauseTime = 0;

    _dataSource = NULL;

    _loadedCurve = NULL;
}

void AmpPlot::setDataSource(DataSource *source)
{
    qDebug() << "Setting Data Source...";
    if (_dataSource)
        delete _dataSource;
    _dataSource = source;
    connect(_dataSource, SIGNAL(dataRead(double,double)), this, SLOT(dataRead(double, double)));
}

void AmpPlot::startRead()
{
    if (_dataSource)
    {
        _pauseTime = 0;
        _data.clear();
        _dataTime.clear();
        _dataSource->stopRead();
        _dataSource->startRead();
        _meanTime.first() = 0.0;
        _meanTime.last() = 0.0;
        _currentMeanData.clear();

        _time->restart();
        _plot->setAxisScale(QwtPlot::xBottom,0, _xMax);
    }
}

void AmpPlot::pauseRead()
{
    if (_dataSource)
        _dataSource->stopRead();
    _pauseTime = _dataTime.last();
}

void AmpPlot::unpauseRead()
{
    if(_dataSource)
        _dataSource->startRead();
}

QwtPlotCurve *AmpPlot::getCurve()
{
    return _dataCurve;
}

void AmpPlot::dataRead(double value, double time)
{
    double size;
    _data.append(value);
    _dataTime.append(time+_pauseTime);

    size = _data.size();
//    lastMean = _mean;
    _mean = (1 - (1/size))*_mean + (1/size)*value;
//    _sd = ((size-1) * _sd + (value -_mean)*(value - lastMean))* (1 / size);

    _meanData.first() = _mean;
    _meanData.last() = _mean;
    _meanTime.last() = _dataTime.last();

    _currentMeanData.append(_mean);

    _dataCurve->setRawSamples(_dataTime.data(), _data.data(), _data.size());
    _meanCurve->setRawSamples(_meanTime.data(), _meanData.data(), _meanTime.size());
    _currentMeanCurve->setRawSamples(_dataTime.data(), _currentMeanData.data(), _dataTime.size());

    emit meanChanged(_mean);

    _plot->setAxisScale(QwtPlot::xBottom,_dataTime.last() - _xMax, _dataTime.last());
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

void AmpPlot::saveToCSV(QString fileName)
{
    QFileInfo fileInfo(fileName);
    if(fileInfo.suffix() == NULL)
        fileName += ".csv";
    QFile *file = new QFile(fileName);
    if(file->open(QIODevice::WriteOnly))
    {
        QTextStream fileStream(file);
        fileStream << "Time,Value\n";
        for(int i = 0; i < _data.size(); ++i)
            fileStream << QString().number(_dataTime.data()[i]) + "," + QString().number(_data.data()[i]) + "\n";
    }

    delete(file);
}

void AmpPlot::loadFromCSV(QString fileName)
{
    QFile *file;

    file = new QFile(fileName);

    if(_loadedCurve)
        _loadedCurve->detach();
        delete(_loadedCurve);
    _loadedData.clear();
    _loadedTime.clear();

    if(file->open(QIODevice::ReadOnly))
    {
        QTextStream fileStream(file);
        QString line;
        if((line = fileStream.readLine()) != NULL)
        {
            QStringList tokens;
            tokens = line.split(",");
            if (tokens.size() < 2)
                return;
            while(!fileStream.atEnd())
            {
                tokens = fileStream.readLine().split(",");
                if(tokens.size() < 2)
                    return;
                _loadedTime.append(tokens.first().toDouble());
                _loadedData.append(tokens.last().toDouble());
            }
        }
    }
    QFileInfo pathInfo(fileName);
    _loadedCurve = new QwtPlotCurve(pathInfo.baseName());
    _loadedCurve->attach(_plot);
    _loadedCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    _loadedCurve->setPen(QPen(Qt::darkGreen));
    _loadedCurve->setVisible(_plot);

    _loadedCurve->setRawSamples(_loadedTime.data(), _loadedData.data(), _loadedData.size());

    showCurve(_loadedCurve, true);

    _plot->replot();
}
