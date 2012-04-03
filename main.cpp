#include <QtGui/QApplication>
#include <QLayout>
#include <QLabel>
#include "mainwindow.h"

#include "ampplot.h"
#include "randomdata.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget vBox;
    vBox.setWindowTitle("Herp");

    AmpPlot *plot = new AmpPlot(&vBox);
    plot->setTitle("Pan with the mouse\nResize with the wheel");

    RandomData *source = new RandomData();
    plot->setDataSource(source);

    QVBoxLayout *layout = new QVBoxLayout(&vBox);
    layout->addWidget(plot);

    vBox.resize(400,400);
    vBox.show();

    return a.exec();
}
