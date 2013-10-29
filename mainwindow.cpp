#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _mutex(new QMutex()), _thread(0)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _mutex;
}

void MainWindow::on_startStopButton_clicked()
{
    QMutexLocker lock(_mutex);
    if (_thread) {
        _thread->terminate();
        _thread->wait();
        delete _thread;
        _thread = 0;
    }
    else {
        _thread = new FFTThread(this);
        _thread->start();
    }
}
