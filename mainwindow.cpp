#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _amp = new AmpPlot(ui->qwtPlot);

    _amp->setDataSource(new RandomData());
    _amp->startRead();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRandom_triggered()
{
    ui->actionConstant->setChecked(false);
    _amp->setDataSource(new RandomData());
    _amp->startRead();
}

void MainWindow::on_actionConstant_triggered()
{
    ui->actionRandom->setChecked(false);
    _amp->setDataSource(new ConstantData());
    _amp->startRead();
}

void MainWindow::on_actionAntialiasing_triggered()
{
    _amp->getCurve()->setRenderHint(QwtPlotItem::RenderAntialiased,ui->actionAntialiasing->isChecked());
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionPause_triggered()
{
        if(ui->actionPause->isChecked())
            _amp->pauseRead();
        else
            _amp->unpauseRead();
}

void MainWindow::on_actionReset_triggered()
{
    _amp->startRead();
    ui->actionPause->setChecked(false);
}

void MainWindow::on_actionSave_triggered()
{
    QStringList fileName;
    QDir dir;
    QFileDialog *dialog = new QFileDialog(this,"Save File...", dir.homePath(),"CSV Files(*.csv)");
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    if(dialog->exec())
        fileName = dialog->selectedFiles();
    if (!fileName.isEmpty())
    {
        qDebug() << fileName.first();
        _amp->saveToCSV(fileName.first());
    }
    else
        qDebug() << "No File Selected";
    delete(dialog);
}

void MainWindow::on_actionLoad_triggered()
{
    QStringList fileName;
    QDir dir;
    QFileDialog *dialog = new QFileDialog(this,"Open File...", dir.homePath(), "CSV Files(*.csv)");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    if(dialog->exec())
        fileName = dialog->selectedFiles();
    if (!fileName.isEmpty())
    {
        qDebug() << fileName.first();
        _amp->loadFromCSV(fileName.first());
    }
    else
        qDebug() << "No File Selected";
    delete(dialog);
}
