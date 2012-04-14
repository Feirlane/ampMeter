#ifndef AMPPLOT_H
#define AMPPLOT_H

#include <QObject>
#include <QTime>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_draw.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_scale_widget.h>

#include "datasource.h"

#define DEFAULT_X_MAX 10000
#define DEFAULT_Y_MAX 100

class AmpPlot : public QObject
{
    Q_OBJECT
public:
    AmpPlot(QwtPlot *plot);
    void setDataSource(DataSource *source);
    void startRead();
    void pauseRead();
    void unpauseRead();
    QwtPlotCurve *getCurve();
    void saveToCSV(QString fileName);
    void loadFromCSV(QString fileName);
signals:
    void meanChanged(double mean);

private slots:
    void dataRead(double value, double time);
    void pannerMoved(int dx, int dy);
    void showCurve(QwtPlotItem *, bool on);

private:
    QwtPlot *_plot;
    QwtPlotCurve *_dataCurve;
    QwtPlotCurve *_meanCurve;
    QwtPlotCurve *_currentMeanCurve;
    QwtPlotCurve *_loadedCurve;
    QwtPlotGrid *_grid;
    QwtPicker *_picker;
    QwtPlotPanner *_panner;
    QwtPlotMagnifier *_magnifier;
    DataSource *_dataSource;
    QVector<double> _data;
    QVector<double> _dataTime;
    QVector<double> _meanData;
    QVector<double> _meanTime;
    QVector<double> _currentMeanData;
    QVector<double> _loadedData;
    QVector<double> _loadedTime;
    QTime *_time;
    double _xMax;
    double _yMax;
    double _mean;
    double _sd; //Standard deviation
    double _pauseTime;

};

#endif // AMPPLOT_H
