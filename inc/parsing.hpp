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

class ParsedFile
{
	private:
		int	tempo;
		char*	tracks;
		t_notes *notes;
	public:
		int gettempo();
		ParsedFile(std::string file_name);

}