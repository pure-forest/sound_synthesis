#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <cstring>

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

class ParsedFile
{
private:
    int tempo;
    std::vector<t_track> tracks;
    
    int currentOctave;
    float currentDuration;

public:
    ParsedFile();
    ~ParsedFile();
    
    int parseFile(const std::string& filename);
    void printSummary() const;
    void cleanup();
    
    // Getters for accessing parsed data
    int getTempo() const;
    const std::vector<t_track>& getTracks() const;
    const t_track* getTrack(int trackIndex) const;
    int getTrackCount() const;
    
    //work with notes 
    static std::string noteToString(const t_notes* note);
    static float getTotalDuration(const t_notes* notes);
    static int countNotes(const t_notes* notes);
    static t_notes* findNoteByPosition(const t_notes* notes, float startTime);

private:
    void trim(std::string& str);
    int isCommentLine(const std::string& line) const;
    int parseTempoLine(const std::string& line);
    int parseTracksLine(const std::string& line);
    int parseTrackLine(const std::string& line);
    void parseNotesForTrack(int trackIndex, const std::string& notesStr);
    int parseNoteToken(const std::string& token, t_notes& note);
    void resetLineState();
    void addNoteToTrack(int trackIndex, const t_notes& note);
};

int parseMusicFile(const std::string& filename);

#endif