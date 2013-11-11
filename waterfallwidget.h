#ifndef WATERFALLWIDGET_H
#define WATERFALLWIDGET_H

#include <QWidget>
#include <QPaintEvent>

#include <QImage>
#include <QMutex>
#include <QMutexLocker>

#include "fft_buffer.h"

class WaterfallWidget : public QWidget
{
    Q_OBJECT
protected:
    QMutex* _image_mutex;
    QImage* _image;

public:
    explicit WaterfallWidget(QWidget *parent = 0);
    ~WaterfallWidget();
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

signals:

public slots:
    void updateWithData(FFTBuffer *fft_buffer);
};

#endif // WATERFALLWIDGET_H
