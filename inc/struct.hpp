#pragma once
#include <iostream>

typedef struct s_notes
{
    char pitch;
    char alter;
    int octave;
    float duration;
    struct s_notes *next;

} t_notes;

typedef struct s_track
{
    int trackNumber;
    std::string instrument;
    t_notes* notes;
} t_track;

enum	type
{
	SINE,
	TRIANGLE,
	SQUARE,
	SAW,
};

struct Note 
{
    double frequency;  
    double duration;
};

struct Track 
{
    std::vector<Note> notes;
    int waveType;       // 0=sine, 1=square, 2=triangle, 3=saw
};
