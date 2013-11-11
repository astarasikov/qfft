#include "audiobuffer.h"
#include <algorithm>

qint64 AudioBuffer::readData(char *data, qint64 maxlen)
{
    QMutexLocker lock(_mutex);
    Q_ASSERT(_head < _buffer_size);
    qint64 len = maxlen < _buffer_size ? maxlen : _buffer_size;
    qint64 space_in_buffer = _buffer_size - _head;
    qint64 chunk_size_end = qMin(len, space_in_buffer);
    std::copy(_buffer + _head, _buffer + _head + chunk_size_end, data);
    _head += chunk_size_end;
    _head %= _buffer_size;
    if (len > space_in_buffer) {
        qint64 rem = len - space_in_buffer;
        std::copy(_buffer, _buffer + rem, data + space_in_buffer);
        _head = rem;
    }
    return len;
}

qint64 AudioBuffer::writeData(const char *data, qint64 len)
{
    QMutexLocker lock(_mutex);
    Q_ASSERT(_head < _buffer_size);
    qint64 space_in_buffer = _buffer_size - _head;
    if (len >= _buffer_size) {
        _head = 0;
        std::copy(data, data + _buffer_size, _buffer);
    }
    else if (len > space_in_buffer) {
        qint64 remaining_data = len - space_in_buffer;
        const char *space_end = data + space_in_buffer;
        std::copy(data, space_end, _buffer + _head);
        std::copy(space_end, space_end + remaining_data, _buffer);
        _head = remaining_data;
    }
    else {
        std::copy(data, data + len, _buffer + _head);
        _head += len;
        _head %= _buffer_size;
    }

    return len;
}
