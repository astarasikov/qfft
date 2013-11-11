#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::toggleProcessing()
{
    QMutexLocker lock(_mutex);
    if (_thread) {
        _thread->terminate();
        _thread->wait();
        delete _thread;
        _thread = 0;
    }
    else {
        unsigned sampleRate = ui->sampleRateComboBox->currentText().toInt(0);
        unsigned interval = ui->intervalSpinBox->value();
        unsigned bufferSize = ui->bufferSizeSpinBox->value();

        _thread = new FFTThread(this, interval, sampleRate, bufferSize);
        connect(_thread, SIGNAL(fftReady(FFTBuffer*)),
                ui->waterfallWidget, SLOT(updateWithData(FFTBuffer*)));
        _thread->start();
    }
}

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
    toggleProcessing();
}
