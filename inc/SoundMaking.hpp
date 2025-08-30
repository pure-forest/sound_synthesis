#pragma once

#include "../inc/parsing.hpp"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <cmath>
#include "struct.hpp"

class parsing;

struct Note
{
    double frequency;
    double duration;
};

struct Track
{
    std::vector<Note> notes;
    int waveType;
};

enum	type
{
	SINE,
	TRIANGLE,
	SQUARE,
	SAW,
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
		pa_sample_spec 	_sample_spec;
		int			 	_error;
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
		//constructor destrucor

		/**
		 * Constructor of sound making class. Initiate for the pa_ related variables to have intial value and create a sample pointer
		 */
		SoundMaking();

		/**
		 * Destructor of sound making class: clean up all the pa_sample related variables and print a message indicating sound playing finished
		 */
		~SoundMaking();
		SoundMaking(ParsedFile& parser);
		//wave related functions

	//	void  	makeSound(const Note *notes, int waveType);
	//	void 	makeSoundStereo(const Track& leftTrack, const Track& rightTrack);
	//	Track	ParsedToSound(const t_track& parsedTrack, int tempo, int wavetype);
		void printTrack() const;
};
