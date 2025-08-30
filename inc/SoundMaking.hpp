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
		const double		_amplitude = 0.1;
		const int 			_sampleRate = 44100;
		const int 			_bufferSize = 1024;
		pa_simple 			*_pa;
		pa_sample_spec 		_sample_spec;
		int			 		_error;
		std::vector<Track>	_song;
		int					_numOfTrack;
		//calculation for different types of wave, not useful outside of class
		float	triangleWave(double frequency, double t);
		float	squareWave(double frequency, double t);
		float	sineWave(double frequency, double t);
		float	sawWave(double frequency, double t);
		float 	generateWaveSample(int waveType, double frequency, int sampleIndex, int sampleRate);
		Track	ParsedToSound(const t_track& parsedTrack, int tempo, int wavetype);
	public:
		//constructor destrucor
		SoundMaking() = delete;
		SoundMaking(ParsedFile& parser);
		/**
		 * Destructor of sound making class: clean up all the pa_sample related variables and print a message indicating sound playing finished
		 */
		~SoundMaking();
		/**
		 * make sound function that uses all song vector to generate samples to make sounds. 
		 */
		void  	makeSound();
		void 	printTrack() const;
};

double	calculateFrequency(char pitch, char alteration, int octave);
double	beatsToSeconds(double beats, int tempo);
