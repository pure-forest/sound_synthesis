#pragma once

#include "../inc/parsing.hpp"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <cmath>

class parsing;

enum	type
{
	TRIANGLE,
	SINE,
	SQUARE,
	SAW,
};

class SoundMaking
{
	private:
		double	_frequency;
		int		_index;
		int		_rate;
		float	_wave;
		pa_simple *_pa;
	public:
		//constructor destructor
		SoundMaking();
		SoundMaking(parsing &obj, int type);
		~SoundMaking();

		//wave related functions
		float triangleWave();
		float squareWave();
		float sinWave();
		float sawWave();
		void  makeSound();

};
