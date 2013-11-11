#include "waterfallwidget.h"

#include <QPainter>
#include <QDebug>

#include <cstring>

static const QImage::Format ImageFormat = QImage::Format_RGB32;

WaterfallWidget::WaterfallWidget(QWidget *parent) :
    QWidget(parent), _image_mutex(new QMutex()),
    _image(new QImage(size(), ImageFormat))
{
}

WaterfallWidget::~WaterfallWidget()
{
    delete _image;
}

void WaterfallWidget::paintEvent(QPaintEvent *event)
{
    QMutexLocker lock(_image_mutex);
    QPainter painter(this);
    painter.drawImage(0, 0, *_image);
}

void WaterfallWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QMutexLocker lock(_image_mutex);
    delete _image;
    _image = new QImage(event->size(), ImageFormat);
    _image->fill(Qt::blue);
}

void WaterfallWidget::updateWithData(FFTBuffer *fft_buffer)
{
    {
        QMutexLocker lock(_image_mutex);
        //shift the image one row up
        uchar* bits = _image->bits();
        size_t bytes_per_line = _image->bytesPerLine();
        size_t old_size = bytes_per_line * (_image->height() - 1);
        ::memmove(bits, bits + bytes_per_line, old_size);

        {
            QMutexLocker flock(fft_buffer->_mutex);
            for (size_t i = 0; i < qMin((qint64)_image->width(), fft_buffer->_size);
                 i++)
            {
                unsigned color = (unsigned)(0xfffff * fft_buffer->_data[i]);
                if (i % 100 == 0) {
                    //for testing purposes, draw a line for each 100Hz
                    color = 0xffff00;
                }
                ((unsigned*)(bits + old_size))[i] = color;
            }
        }
    }
    repaint();
}
