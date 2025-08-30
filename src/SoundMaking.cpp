#include "../inc/SoundMaking.hpp"

SoundMaking::SoundMaking()
{
    _sample_spec.format = PA_SAMPLE_FLOAT32LE;
    _sample_spec.rate = _sampleRate;
    _sample_spec.channels = 2;
    _pa = pa_simple_new(nullptr, "Minitsynth", PA_STREAM_PLAYBACK, nullptr, "playback", &_sample_spec, nullptr, nullptr, &_error);
    if (!_pa)
    {
        std::cerr << "PulseAudio error: " << pa_strerror(_error) << std::endl;
        exit(1);
    }
}

float SoundMaking::sineWave(double frequency, double t)
{
     //from forumla: y(t)=sin(2πft)
    return static_cast<float>(_amplitude * sin(2.0 * M_PI * frequency * t));
}

float SoundMaking::triangleWave(double frequency, double t)
{
   //from formula: y(t)=2⋅2⋅(ft−⌊ft+0.5⌋) −1
   return static_cast<float>(_amplitude * (2.0 * fabs(2.0 * (frequency * t - floor(frequency * t + 0.5))) - 1.0));
}

float SoundMaking::squareWave(double frequency, double t)
{
    //from formulla: y(t)=sign(sin(2πft)) 
    //sign means for sign(a) if a > 0, sign(a) += 1; a < 0, sign(a) -= 1；a=0, sign(a) = 0;
    return static_cast<float>(_amplitude * ((sin(2.0 * M_PI * frequency * t) >= 0.0) ? 1.0 : -1.0));

}
float SoundMaking::sawWave(double frequency, double t)
{
   //from formula: y(t)=2(ft−⌊ft⌋)−1
   // floor means substract the integer part of a double but for floor(-1.2) = -2; getting the greater part
    return static_cast<float>(_amplitude * (2.0 * (frequency * t - floor(frequency * t)) - 1.0));
}

SoundMaking::~SoundMaking() 
{
    pa_simple_drain(_pa, &_error);
    pa_simple_free(_pa);
}

/*
void SoundMaking::makeSound(const std::vector<Note>& score, int waveType) {
    for (const auto& note : score) {
        int totalSamples = static_cast<int>(note.duration * _sampleRate);

        for (int i = 0; i < totalSamples; i += _bufferSize) 
        {
            int blockSize = std::min(_bufferSize, totalSamples - i);
            for (int j = 0; j < blockSize; ++j) 
            {
                double t = static_cast<double>(i + j) / _sampleRate;
                switch (waveType) 
                {
                    case 0: _buffer[j] = sineWave(note.frequency, t); break;
                    case 1: _buffer[j] = squareWave(note.frequency, t); break;
                    case 2: _buffer[j] = triangleWave(note.frequency, t); break;
                    case 3: _buffer[j] = sawWave(note.frequency, t); break;
                    default: _buffer[j] = sineWave(note.frequency, t); break;
                }
            }
            if (pa_simple_write(_pa, _buffer, blockSize * sizeof(float), &_error) < 0) {
                std::cerr << "PulseAudio write error: " << pa_strerror(_error) << std::endl;
                return;
            }
        }
    }
    std::cout << "Finished playing sequence." << std::endl;
}

void SoundMaking::makeSoundStereo(const Track& leftTrack, const Track& rightTrack) {
    size_t leftNoteIndex = 0, rightNoteIndex = 0;
    double leftTime = 0.0, rightTime = 0.0;
    int leftSampleIndex = 0, rightSampleIndex = 0;

    float _buffer[_bufferSize * 2]; // *2 for stereo

    while (leftNoteIndex < leftTrack.notes.size() || rightNoteIndex < rightTrack.notes.size()) {
        int blockSize = _bufferSize;

        for (int j = 0; j < blockSize; ++j) {
            float leftSample = 0.0f;
            float rightSample = 0.0f;

            // Left track
            if (leftNoteIndex < leftTrack.notes.size()) {
                const Note& note = leftTrack.notes[leftNoteIndex];
                leftSample = generateWaveSample(leftTrack.waveType, note.frequency, leftSampleIndex, _sampleRate);
                leftTime = static_cast<double>(leftSampleIndex) / _sampleRate;
                leftSampleIndex++;
                if (leftSampleIndex >= note.duration * _sampleRate) {
                    leftSampleIndex = 0;
                    leftNoteIndex++;
                }
            }

            // Right track
            if (rightNoteIndex < rightTrack.notes.size()) {
                const Note& note = rightTrack.notes[rightNoteIndex];
                rightSample = generateWaveSample(rightTrack.waveType, note.frequency, rightSampleIndex, _sampleRate);
                rightTime = static_cast<double>(rightSampleIndex) / _sampleRate;
                rightSampleIndex++;
                if (rightSampleIndex >= note.duration * _sampleRate) {
                    rightSampleIndex = 0;
                    rightNoteIndex++;
                }
            }

            // Interleave samples for stereo
            _buffer[2*j]   = leftSample;
            _buffer[2*j+1] = rightSample;
        }

        if (pa_simple_write(_pa, _buffer, blockSize * 2 * sizeof(float), &_error) < 0) {
            std::cerr << "PulseAudio write error: " << pa_strerror(_error) << std::endl;
            return;
        }
    }

    pa_simple_drain(_pa, &_error);
    std::cout << "Finished playing stereo tracks." << std::endl;
}

*/
float SoundMaking::generateWaveSample(int waveType, double frequency, int sampleIndex, int sampleRate) 
{
    double t = static_cast<double>(sampleIndex) / sampleRate;
    switch (waveType) 
    {
        case 0: return sineWave(frequency, t);
        case 1: return squareWave(frequency, t);
        case 2: return triangleWave(frequency, t);
        case 3: return sawWave(frequency, t);
        default: return sineWave(frequency, t);
    }
}