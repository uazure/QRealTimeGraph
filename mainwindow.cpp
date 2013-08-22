#include <cstdlib>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    min = ui->minSpinBox->value();
    max = ui->maxSpinBox->value();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateButton_clicked()
{
    //generate random double value;
    double a = (double)min + (double)rand()/((double)RAND_MAX/(max-min));
    ui->graph->setCurrentValue(a, 0, this);
    ui->graph->update();
}

void MainWindow::on_maxSpinBox_valueChanged(double arg1)
{
    max=arg1;
}

void MainWindow::on_minSpinBox_valueChanged(double arg1)
{
    min=arg1;
}
