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
		pa_sample_spec 	_sample_spec;
		int			 	_error;
		std::vector<Track>	song;
		int				_numOfTrack;
		std::vector<pa_simple*> streams;
		//calculation for different types of wave;
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
		//wave related functions

		void  	makeSound();
		void 	printTrack() const;
};
