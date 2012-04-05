#ifndef AMPPLOT_H
#define AMPPLOT_H

#include <QObject>
#include <QTime>
#include <QVector>

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

#include "datasource.h"

#define DEFAULT_X_MAX 10000
#define DEFAULT_Y_MAX 100

class AmpPlot
{
    Q_OBJECT
public:
    AmpPlot(QWidget * = 0);
    void setDataSource(DataSource *source);
    void startRead();
    void pauseRead();
    void unpauseRead();
    QwtPlotCurve *getCurve();
signals:
    void meanChanged(double mean);

private slots:
    void dataRead(double value);
    void pannerMoved(int dx, int dy);

private:
    QwtPlot *_plot;
    QwtPlotCurve *_curve;
    QwtPlotCurve *_mean;
    QwtPlotCurve *_currentMean;
    QwtPlotGrid *_grid;
    QwtPicker *_picker;
    QwtPlotPanner *_panner;
    QwtPlotMagnifier *_magnifier;
    DataSource *_dataSource;
    QVector<double> _data;
    QVector<double> _timeData;
    QTime *_time;
    double _xMax;
    double _yMax;
    double _mean;
    double _sd; //Standard deviation

};

#endif // AMPPLOT_H
