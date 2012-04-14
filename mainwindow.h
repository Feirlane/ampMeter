#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>

#include "datasource.h"
#include "randomdata.h"
#include "constantdata.h"
#include "ampplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void on_actionRandom_triggered();

    void on_actionAntialiasing_triggered();

    void on_actionQuit_triggered();

    void on_actionPause_triggered();

    void on_actionReset_triggered();

    void on_actionConstant_triggered();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    AmpPlot *_amp;
};

#endif // MAINWINDOW_H
