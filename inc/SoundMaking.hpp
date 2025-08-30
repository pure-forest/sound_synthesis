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
		//calculation for different types of wave;
		float	triangleWave(double frequency, double t);
		float	squareWave(double frequency, double t);
		float	sineWave(double frequency, double t);
		float	sawWave(double frequency, double t);
		float 	generateWaveSample(int waveType, double frequency, int sampleIndex, int sampleRate);
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

		/**
		 * A simple sound making function that takes a sequence of notes and play it accordingly into a single channel.
		 * 
		 * @param score
		 * 			a vector points to an array of note struct, each of the note contains durantion and frequency to make sound
		 * @param waveType
		 * 			an int from enum type, each number indicates a type of wave (sine, triangle, square, saw)
		 */
		void  	makeSound(const std::vector<Note>& score, int waveType);

		/**
		 * A more advanced sound making function that can take up to 2 tracks and play the sounds simutaneously.
		 * @param leftTrack
		 * 		a struct contains a vector of notes and and int indicating type of wave.
		 * @param rightTrack
		 * 		a struct contains a vector of notes and and int indicating type of wave.
		 * 
		 */
		void 	makeSoundStereo(const Track& leftTrack, const Track& rightTrack);
		
		

};
