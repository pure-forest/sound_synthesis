#pragma once

#include "../inc/parsing.hpp"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <cmath>
#include "struct.hpp"

class parsing;


class SoundMaking
{
	private:
		//const value
		const double	_amplitude = 0.5;
		const int 		_sampleRate = 44100;
		const int 		_bufferSize = 1024;
		//pa related variables  
		pa_simple 		*_pa;
		pa_sample_spec _sample_spec;
		int			   _error;
		float			_buffer[1024];
		std::vector<Track>	song;

		//calculation for different types of wave;
		float	triangleWave(double frequency, double t);
		float	squareWave(double frequency, double t);
		float	sineWave(double frequency, double t);
		float	sawWave(double frequency, double t);
		float 	generateWaveSample(int waveType, double frequency, int sampleIndex, int sampleRate);
		Track	ParsedToSound(const t_track& parsedTrack, int tempo, int wavetype);
	public:
		//constructor destructor
		SoundMaking();
		~SoundMaking();
		SoundMaking(ParsedFile& parser);
		//wave related functions
		
	//	void  	makeSound(const Note *notes, int waveType);
	//	void 	makeSoundStereo(const Track& leftTrack, const Track& rightTrack);
	//	Track	ParsedToSound(const t_track& parsedTrack, int tempo, int wavetype);
		void printTrack() const;
};
