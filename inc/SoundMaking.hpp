#pragma once

#include "../inc/parsing.hpp"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <cmath>

class parsing;

struct Note 
{
    double frequency;  
    double duration; 
};

enum	type
{
	SINE,
	TRIANGLE,
	SQUARE,
	SAW,
};

struct Track 
{
    std::vector<Note> notes;
    int waveType;       // 0=sine, 1=square, 2=triangle, 3=saw
};

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
		//calculation for different types of wave;
		float	triangleWave(double frequency, double t);
		float	squareWave(double frequency, double t);
		float	sineWave(double frequency, double t);
		float	sawWave(double frequency, double t);
		float 	generateWaveSample(int waveType, double frequency, int sampleIndex, int sampleRate);
	public:
		//constructor destructor
		SoundMaking();
		~SoundMaking();

		//wave related functions
		
		void  	makeSound(const std::vector<Note>& score, int waveType);
		void 	makeSoundStereo(const Track& leftTrack, const Track& rightTrack);
		
		

};
