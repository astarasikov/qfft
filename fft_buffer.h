#ifndef FFT_BUFFER_H
#define FFT_BUFFER_H

#include <QMutex>
#include <QDebug>
#include <algorithm>
#include <complex>
#include "dsp.h"

struct FFTBuffer {
    qint64 _size;
    float *_data;
    std::complex<float> *_fft_data;
    QMutex *_mutex;

    FFTBuffer(size_t count) :
        _size(next_power_of_two(count)),
        _data(new float[_size]()),
        _fft_data(new std::complex<float>[_size]()),
        _mutex(new QMutex())
    {}
    ~FFTBuffer() {
        delete[] _data;
        delete[] _fft_data;
    }

    void process(size_t count) {
        const float eps = 1e-6;

        //multiply the data by the windowing function (Hann)
        for (size_t i = 0; i < count; i++) {
            _data[i] *= (0.5 * (1 - cos(2.0f * M_PI * i / (count - 1))));
        }

        if (count < _size) {
            std::fill(_data + count, _data + _size, 0.0f);
        }
        std::copy(_data, _data + _size, _fft_data);
        fft(_fft_data, _size, false);

        size_t fft_size = _size / 2;
        std::transform(_fft_data, _fft_data + fft_size, _data,
            static_cast<float(*)(const std::complex<float>&)>(std::abs));

        //reset DC coefficient. If we set this to zero, then
        //we'll need to calculate dB power starting from index 1
        //to avoid dealing with NaN
        _data[0] = _data[1];

        float minVal = 0, maxVal = 0;
        for (size_t i = 0; i < fft_size; i++) {
            maxVal = qMax(_data[i], maxVal);
            minVal = qMin(_data[i], minVal);
        }
        /* if we calculate the log of zero, we'll get a NaN */
        if (std::abs(minVal) < eps) {
            minVal = eps;
        }
        maxVal = qMax(maxVal, float(1e-6));

        float dbMin = 10 * log10f(minVal / maxVal);
        qDebug() << minVal << " " << maxVal;

        for (size_t i = 0; i < fft_size; i++) {
            float db = 10 * log10f(_data[i] / maxVal);
            _data[i] = db / dbMin;
        }

        maxVal = minVal = 0.0;
        for (size_t i = 0; i < fft_size; i++) {
            maxVal = qMax(maxVal, _data[i]);
            minVal = qMin(minVal, _data[i]);
        }
        qDebug() << minVal << " " << maxVal;
    }
};

#endif // FFT_BUFFER_H
