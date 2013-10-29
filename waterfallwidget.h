#ifndef WATERFALLWIDGET_H
#define WATERFALLWIDGET_H

#include <QWidget>
#include <QPaintEvent>

#include <QImage>
#include <QMutex>
#include <QMutexLocker>

class WaterfallWidget : public QWidget
{
    Q_OBJECT
protected:
    QMutex* _imageMutex;
    QImage* _image;

public:
    explicit WaterfallWidget(QWidget *parent = 0);
    ~WaterfallWidget();
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void updateWithData(float *data, qint64 count, qint64 sampleRate);

signals:

public slots:

};

#endif // WATERFALLWIDGET_H
