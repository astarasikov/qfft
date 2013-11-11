#ifndef FFTTHREAD_H
#define FFTTHREAD_H

#include <QThread>
#include <QTimer>

#include "audiobuffer.h"
#include "fft_buffer.h"
#include <QAudioInput>

class FFTThread : public QThread
{
    Q_OBJECT
protected:
    unsigned _sample_rate;
    unsigned _buffer_size;
    QTimer* _timer;
    AudioBuffer *_audio_buffer;
    QAudioInput *_audio_input;
    FFTBuffer *_fft_buffer;

    void initAudio();
public:
    FFTThread(QObject *parent = 0, unsigned period = 1000,
              unsigned sampleRate = 8000, unsigned bufferSize = 400,
              unsigned fftSize = 2000);
    ~FFTThread();

signals:
    void fftReady(FFTBuffer *buffer);

public slots:
    void processAudio();
};

#endif // FFTTHREAD_H
