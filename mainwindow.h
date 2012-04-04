#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
