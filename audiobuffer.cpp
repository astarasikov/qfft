#include "audiobuffer.h"
#include <algorithm>

qint64 AudioBuffer::readData(char *data, qint64 maxlen)
{
    qDebug() << __func__ << " " << maxlen;
    QMutexLocker lock(_mutex);
    qint64 used_length = maxlen % _bufferSize;

}

qint64 AudioBuffer::writeData(const char *data, qint64 len)
{
    qDebug() << __func__ << " " << len;

    QMutexLocker lock(_mutex);
    qint64 used_length = (len % _bufferSize);
    qint64 space_in_buf = (_bufferSize - _head);

    const char *data_start = data + (len - used_length);
    const char *data_end = data_start + len;

    if (used_length <= space_in_buf) {
        std::copy(data_start, data_end, _buffer + _head);
        _head += used_length;
    }
    else {
        const char *first_chunk_end = data_start + space_in_buf;
        qint64 second_chunk_size = used_length - space_in_buf;
        std::copy(data_start, first_chunk_end, _buffer + _head);
        std::copy(first_chunk_end,
                  first_chunk_end + second_chunk_size,
                  _buffer);
        _head = second_chunk_size;
    }
    return len;
}
