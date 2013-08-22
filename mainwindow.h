#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qrealtimegraph.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    double min, max;
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_generateButton_clicked();

    void on_maxSpinBox_valueChanged(double arg1);

    void on_minSpinBox_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
