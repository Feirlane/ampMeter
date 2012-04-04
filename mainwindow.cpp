#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->plotFrame->setDataSource(new RandomData());
    ui->plotFrame->startRead();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_plotFrame_destroyed(QObject *arg1)
{

}

void MainWindow::on_actionRandom_triggered()
{
    ui->plotFrame->setDataSource(new RandomData());
}

void MainWindow::on_actionStart_triggered()
{
    ui->plotFrame->startRead();
}

void MainWindow::on_actionStop_triggered()
{
    ui->plotFrame->stopRead();
}

void MainWindow::on_action_Pause_triggered()
{
    if(ui->action_Pause->isChecked())
        ui->plotFrame->stopRead();
    else
        ui->plotFrame->restartRead();
}

void MainWindow::on_actionAntialiasing_triggered()
{
    ui->plotFrame->getCurve()->setRenderHint(QwtPlotItem::RenderAntialiased,ui->actionAntialiasing->isChecked());
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}
