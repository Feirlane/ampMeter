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

void MainWindow::on_actionRandom_triggered()
{
    ui->plotFrame->setDataSource(new RandomData());
}

void MainWindow::on_actionAntialiasing_triggered()
{
    ui->plotFrame->getCurve()->setRenderHint(QwtPlotItem::RenderAntialiased,ui->actionAntialiasing->isChecked());
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionPause_triggered()
{
        if(ui->actionPause->isChecked())
            ui->plotFrame->pauseRead();
        else
            ui->plotFrame->unpauseRead();
}

void MainWindow::on_actionReset_triggered()
{
    ui->plotFrame->startRead();
}
