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
//#include <qwt_legend.h>
//#include <qwt_legend_item.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>

#include "datasource.h"

#define DEFAULT_X_MAX 10000
#define DEFAULT_Y_MAX 100

class AmpPlot : public QwtPlot
{
    Q_OBJECT
public:
    AmpPlot(QWidget * = 0);
    void setDataSource(DataSource *source);
    void startRead();
    void stopRead();
    void restartRead();
    QwtPlotCurve *getCurve();
signals:
    void meanChanged(double mean);

private slots:
    void dataRead(int value);

private:
    QwtPlotCurve *_curve;
    QwtPlotGrid *_grid;
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
