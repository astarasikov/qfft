#include "waterfallwidget.h"

#include <QPainter>
#include <QDebug>

#include <cstring>

static const QImage::Format ImageFormat = QImage::Format_RGB32;

WaterfallWidget::WaterfallWidget(QWidget *parent) :
    QWidget(parent), _imageMutex(new QMutex()),
    _image(new QImage(size(), ImageFormat))
{
}

WaterfallWidget::~WaterfallWidget()
{
    delete _image;
}

void WaterfallWidget::paintEvent(QPaintEvent *event)
{
    QMutexLocker lock(_imageMutex);
    QPainter painter(this);
    painter.drawImage(0, 0, *_image);
}

void WaterfallWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QMutexLocker lock(_imageMutex);
    delete _image;
    _image = new QImage(event->size(), ImageFormat);
    _image->fill(Qt::blue);
}

void WaterfallWidget::updateWithData(float *data,
                                     qint64 count, qint64 sampleRate)
{
    {
    QMutexLocker lock(_imageMutex);
    uchar* bits = _image->bits();
    size_t bytes_per_line = _image->bytesPerLine();
    size_t old_size = bytes_per_line * (_image->height() - 1);

    ::memmove(bits, bits + bytes_per_line, old_size);
    ::memset(bits + old_size, 0xff, bytes_per_line);
    }
    repaint();
}
