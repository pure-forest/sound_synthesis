#include "../inc/SoundMaking.hpp"

SoundMaking::SoundMaking(ParsedFile& parser)
{
	std::memset(&_sample_spec, 0, sizeof(_sample_spec));
	for (int i = 0; i < parser.getTrackCount(); i++)
	{
		const t_track *track = parser.getTrack(i);
		if (track != nullptr)
		{
			Track soundTrack = ParsedToSound(*track, parser.getTempo(), parser.getWaveType(i));
			_song.push_back(soundTrack);
		}
	}
	_numOfTrack = _song.size();
	std::cout << "number of track : " << _numOfTrack << std::endl;;
	_sample_spec.format = PA_SAMPLE_FLOAT32LE;
    _sample_spec.rate = _sampleRate;
    _sample_spec.channels = 2;
	std::cout << "rate in spec: " << _sample_spec.rate << std::endl;
	if (!pa_sample_spec_valid(&_sample_spec))
	{
		std::cerr << "Non valid spec " << std::endl;
	}
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
   //floor means substract the integer part of a double but for floor(-1.2) = -2; getting the greater part
    return static_cast<float>(_amplitude * (2.0 * (frequency * t - floor(frequency * t)) - 1.0));
}

SoundMaking::~SoundMaking()
{
    pa_simple_drain(_pa, &_error);
    pa_simple_free(_pa);
    std::cout << "Finished playing stereo tracks." << std::endl;
}

float SoundMaking::generateWaveSample(int waveType, double frequency, int sampleIndex, int sampleRate)
{
    double t = static_cast<double>(sampleIndex) / sampleRate;
    switch (waveType)
    {
        case SINE: 
            return sineWave(frequency, t);
        case SQUARE:
            return squareWave(frequency, t);
        case TRIANGLE:
            return triangleWave(frequency, t);
        case SAW:
            return sawWave(frequency, t);
        default:
            return sineWave(frequency, t);
    }
}

void SoundMaking::makeSound()
{
    float _buffer[_bufferSize * 2];
    std::vector<size_t> noteIndices(_numOfTrack, 0);
    std::vector<size_t> sampleIndices(_numOfTrack, 0);

    while (1)
    {
        bool alltrackdone = true;
        //frame level calculation. Going throught all the tracks all together one sample frame
        for (int i = 0; i < _bufferSize; ++i)
        {
            float left_sample = 0.0f;
            float right_sample = 0.0f;
            //going throught all the tracks per frame
            for (int track = 0; track < _numOfTrack; ++track)
            {
                float sampleAmplitude = 0.0f;
                if (track < _song.size() && noteIndices[track] < _song[track].notes.size())
                {
                    const Note& note = _song[track].notes[noteIndices[track]];
                    sampleAmplitude = generateWaveSample(_song[track].waveType, note.frequency,
                                    sampleIndices[track], _sampleRate);
                    //increate frame here
                    sampleIndices[track]++;
                    //reset sample index to zero. increment note index
                    if (sampleIndices[track] >= note.duration * _sampleRate)
                    {
                        sampleIndices[track] = 0;
                        noteIndices[track]++;
                    }
                    alltrackdone = false;
                }
                // Why summing up the amplitutd works? THat's how sounds waves mix together...
                if (track % 2 == 0)
                    left_sample += sampleAmplitude;
                else 
                    right_sample += sampleAmplitude;
            }
            //averaging it into the safe range of [-1, 1]
            left_sample = left_sample / (_numOfTrack / 2.0f);
            right_sample = right_sample /(_numOfTrack / 2.0f);
            //writes into 2 channels to produce sounds
            _buffer[i * 2] = left_sample;
            _buffer[i * 2 + 1] = right_sample;
        }
        if (alltrackdone)
            break;
        if (pa_simple_write(_pa, _buffer, _bufferSize * 2 * sizeof(float), &_error) < 0)
        {
            std::cerr << "PulseAudio write error: " << pa_strerror(_error) << std::endl;
            exit(1);
        }
    }
}

