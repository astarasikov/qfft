#ifndef FFTTHREAD_H
#define FFTTHREAD_H

#include <QThread>
#include <QTimer>

#include "audiobuffer.h"
#include <QAudioInput>

class FFTThread :public QThread
{
    Q_OBJECT
protected:
    unsigned _sample_rate;
    QTimer* _timer;
    AudioBuffer *_audio_buffer;
    QAudioInput *_audio_input;

    void initAudio();
public:
    FFTThread(QObject *parent = 0, unsigned sampleRate = 8000, unsigned period = 1000);
    ~FFTThread();

public slots:
    void processAudio();
};

#endif // FFTTHREAD_H
