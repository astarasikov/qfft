#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QMutexLocker>

#include "fftthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    QMutex* _mutex;
    FFTThread *_thread;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startStopButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
