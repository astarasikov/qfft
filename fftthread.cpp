#include "fftthread.h"
#include <QDebug>

#include <QElapsedTimer>

FFTThread::FFTThread(QObject *parent, unsigned period,
                     unsigned sampleRate, unsigned bufferSize,
                     unsigned fftSize) :
    QThread(parent),
    _sample_rate(sampleRate),
    _buffer_size(bufferSize),
    _timer(new QTimer(0)),
    _audio_buffer(0),
    _audio_input(0),
    _fft_buffer(new FFTBuffer(fftSize))
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
    _audio_input->setBufferSize(_buffer_size);
    _audio_input->start(_audio_buffer);
}

FFTThread::~FFTThread()
{
    if (_audio_input) {
        _audio_input->stop();
    }
    _timer->stop();
    delete _timer;
    delete _audio_input;
    delete _audio_buffer;
    delete _fft_buffer;
}

void FFTThread::processAudio()
{
    QElapsedTimer timer;
    timer.start();

    {
        QMutexLocker(_fft_buffer->_mutex);
        qint64 len = _audio_buffer->readData((char*)_fft_buffer->_data,
               _fft_buffer->_size * sizeof(float));
        _fft_buffer->process(len / sizeof(float));
    }
    qint64 dt = timer.elapsed();
    qDebug() << "processed in " << dt;

    emit fftReady(_fft_buffer);
}
