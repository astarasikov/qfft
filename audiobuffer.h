#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include <QDebug>
#include <QIODevice>
#include <QtGlobal>

#include <QMutex>
#include <QMutexLocker>

class AudioBuffer : public QIODevice
{
    Q_OBJECT
protected:
    qint64 _buffer_size;
    qint64 _head;
    char *_buffer;

    QMutex *_mutex;
public:
    AudioBuffer(qint64 bufferSize) : _buffer_size(bufferSize),
        _head(0),
        _buffer(new char[bufferSize]),
        _mutex(new QMutex())
    {
        std::fill(_buffer, _buffer + _buffer_size, 0);
        open(QIODevice::ReadWrite);
    }

    ~AudioBuffer() {
        delete[] _buffer;
        close();
    }

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
};

#endif // AUDIOBUFFER_H
