#include "fftthread.h"
#include <QDebug>

FFTThread::FFTThread(QObject *parent, unsigned sampleRate,
                     unsigned period) :
    QThread(parent),
    _sample_rate(sampleRate),
    _timer(new QTimer(0)),
    _audio_buffer(0),
    _audio_input(0)
{
    _timer->setInterval(period);
    _timer->moveToThread(this);

    initAudio();

    connect(_timer, SIGNAL(timeout()), this, SLOT(processAudio()));
    connect(this, SIGNAL(started()), _timer, SLOT(start()));
}

void FFTThread::initAudio() {
    QAudioFormat format;
    format.setSampleRate(_sample_rate);
    format.setChannelCount(1);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::Float);
    format.setSampleSize(8 * sizeof(float));

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format)) {
        qDebug() << "Format unsupported";
        return;
    }

    _audio_buffer = new AudioBuffer(sizeof(float) * _sample_rate);
    _audio_input = new QAudioInput(format);

    unsigned bufsize = _sample_rate / 20;
    _audio_input->setBufferSize(bufsize ? bufsize : 100);
    _audio_input->start(_audio_buffer);
}

FFTThread::~FFTThread()
{
    _timer->stop();
    delete _timer;
}

void FFTThread::processAudio()
{
    qDebug() << __func__;
}
