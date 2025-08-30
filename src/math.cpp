
#include <cmath>
#include "../inc/parsing.hpp"
#include "../inc/SoundMaking.hpp"

double	calculateFrequency(char pitch, char alteration, int octave)
{
	int pitchNumber;
	double default_freq = 440.0;
	switch (pitch)
	{
		case 'c':
			pitchNumber = 0;
			break;
		case 'd':
			pitchNumber = 2;
			break;
		case 'e':
			pitchNumber = 4;
			break;
		case 'f':
			pitchNumber = 5;
			break;
		case 'g':
			pitchNumber = 7;
			break;
		case 'a':
			pitchNumber = 9;
			break;
		case 'b':
			pitchNumber = 11;
			break;
		default:
			return (0.0);
	}
	int alt = 0;
	if (alteration == 'b')
		alt = -1;
	else if (alteration == '#')
		alt = 1;
	double midiNote = ((octave + 1) * 12) + pitchNumber + alt;
	float num = (midiNote - 69) / 12.0;
	double frequency = default_freq * std::pow(2.0 , num);
	return (frequency);
}

double beatsToSeconds(double beats, int tempo)
{
	return ((beats * 60.0) / static_cast<double>(tempo));
}



Track SoundMaking::ParsedToSound(const t_track& parsedTrack, int tempo, int wavetype)
{
	Track soundTrack;
	soundTrack.waveType = wavetype;

	const t_notes *parsedNote = parsedTrack.notes;
	while (parsedNote != nullptr)
	{
		Note newNote;
		if (parsedNote->pitch == 'r')
			newNote.frequency = 0.0;
		else
			newNote.frequency = calculateFrequency(parsedNote->pitch, parsedNote->alter, parsedNote->octave);

		newNote.duration = beatsToSeconds(parsedNote->duration, tempo);

		soundTrack.notes.push_back(newNote);
		parsedNote = parsedNote->next;
	}
	return (soundTrack);
}


SoundMaking::SoundMaking(ParsedFile& parser)
{
	std::memset(&_sample_spec, 0, sizeof(_sample_spec));
	for (int i = 0; i < parser.getTrackCount(); i++)
	{
		const t_track *track = parser.getTrack(i);
		if (track != nullptr)
		{
			Track soundTrack = ParsedToSound(*track, parser.getTempo(), parser.getWaveType(i));
			soundTrack.volume = parser.getVolume(i);
			song.push_back(soundTrack);
		}
	}
	_numOfTrack = song.size();
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

void SoundMaking::printTrack() const {
    for (size_t i = 0; i < song.size(); i++) {
        std::cout << "Track " << i + 1 << " (Wave Type: " << song[i].waveType << "):" << std::endl;
        std::cout << "Notes:" << std::endl;

        for (size_t j = 0; j < song[i].notes.size(); j++) {
            const Note& note = song[i].notes[j];
            std::cout << "  " << j + 1 << ". Frequency: " << note.frequency << " Hz";
            std::cout << ", Duration: " << note.duration << " seconds";

            if (note.frequency == 0.0) {
                std::cout << " (REST)";
            }

            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

