
#include "../inc/parsing.hpp"

/*
Vector (std::vector)
A vector is like a smart array that can grow and shrink automatically.

It stores elements in a row (like an array)
You can add/remove elements easily
It keeps track of its size automatically
*/

int safeStringToInt(const std::string& str, int& result) {
    if (str.empty())
        return 0;
    
    char* endptr;
    long value = strtol(str.c_str(), &endptr, 10);
    
    if (endptr == str.c_str() || *endptr != '\0') return 0;
    if (value < -2147483647 || value > 2147483647) return 0;
    
    result = (int)value;
    return (1);
}

// Simple string to float conversion
int safeStringToFloat(const std::string& str, float& result) {
    if (str.empty())
        return 0;
    
    char* endptr;
    float value = strtof(str.c_str(), &endptr);
    
    if (endptr == str.c_str() || *endptr != '\0') return 0;
    
    result = value;
    return 1;
}

// ChatGPT string to num ↑↑↑

int parseMusicFile(const std::string& filename) {
    ParsedFile parser;
    int result = parser.parseFile(filename);
    
    if (result == 1) {
        std::cout << "File parsed successfully!" << std::endl;
        parser.printSummary();
        return (1);
    } else {
        std::cout << "Failed to parse file: " << filename << std::endl;
        return (0);
    }
}

// Constructor
ParsedFile::ParsedFile() : tempo(60), currentOctave(4), currentDuration(1.0f) {}

// Destructor - clean up allocated memory
ParsedFile::~ParsedFile() {
    cleanup();
}

void ParsedFile::cleanup() {
    for (size_t i = 0; i < tracks.size(); i++) {
        t_notes* current = tracks[i].notes;
        while (current != nullptr) {
            t_notes* next = current->next;
            delete current; // delete is an operator that is used to destroy array and non-array(pointer) objects which are dynamically created by the new operator.
            current = next;
        }
        tracks[i].notes = nullptr;
    }
    tracks.clear();
}

int ParsedFile::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << std::endl;
        return (0);
    }

    cleanup(); // Clear any previous data

    std::string line;
    int tempoParsed = 0;
    int tracksParsed = 0;

    while (std::getline(file, line)) {
        trim(line);
        
        if (isCommentLine(line) == 1 || line.empty())
            continue;

        if (tempoParsed == 0) {
            if (parseTempoLine(line) == 1) {
                tempoParsed = 1;
            } else {
                std::cout << "Error: Expected tempo declaration" << std::endl;
                return 0;
            }
            continue;
        }

        if (tracksParsed == 0) {
            if (parseTracksLine(line) == 1) {
                tracksParsed = 1;
            } else {
                std::cout << "Error: Expected tracks declaration" << std::endl;
                return 0;
            }
            continue;
        }

        if (parseTrackLine(line) == 0) {
            std::cout << "Warning: Invalid track line: " << line << std::endl;
        }
    }

    file.close();
    return 1;
}

void ParsedFile::printSummary() const {
    std::cout << "Tempo: " << tempo << " BPM" << std::endl;
    std::cout << "Number of tracks: " << tracks.size() << std::endl;
    
    std::cout << "Instruments: ";
    for (size_t i = 0; i < tracks.size(); i++) {
        if (i > 0)
            std::cout << ", ";
        std::cout << tracks[i].instrument;
    }
    std::cout << std::endl;

    std::cout << "\nNOTES" << std::endl;
    for (size_t i = 0; i < tracks.size(); i++) {
        const t_track& track = tracks[i];
        std::cout << "Track " << track.trackNumber << " (" << track.instrument << "): ";
        
        const t_notes* current = track.notes;
        int noteCount = 0;
        
        while (current != nullptr) {
            std::cout << noteToString(current);
            noteCount++;
            
            if (current->next != nullptr)
                std::cout << " ";
    
            if (noteCount % 8 == 0) {
                if (current->next != nullptr)
                    std::cout << std::endl << "                  ";
            }
            
            current = current->next;
        }
        std::cout << std::endl << std::endl;
    }
}

// Getters for accessing parsed data
int ParsedFile::getTempo() const {
    return (tempo);
}

const std::vector<t_track>& ParsedFile::getTracks() const {
    return (tracks);
}

const t_track* ParsedFile::getTrack(int trackIndex) const {
    if (trackIndex < 0) {
        return (nullptr);
    }
    if (trackIndex >= (int)tracks.size()) {
        return (nullptr);
    }
    return (&tracks[trackIndex]);
}

int ParsedFile::getTrackCount() const {
    return tracks.size();
}

int ParsedFile::getWaveType(int trackIndex) const {
	if (trackIndex < 0 || trackIndex >= (int)tracks.size())
		return (0);
	const std::string& instrument = tracks[trackIndex].instrument;
	if (instrument == "sine")
		return (0);
	if (instrument == "square")
		return (1);
	if (instrument == "triangle")
		return (2);
	if (instrument == "saw")
		return (3);
	return (0);
}

std::string ParsedFile::noteToString(const t_notes* note) {
    std::string result;
    
    if (note->pitch == 'r') {
        result = "r";
    } else {
        result += note->pitch;
        if (note->alter != '\0')
            result += note->alter;
        result += std::to_string(note->octave);
    }
    
    result += "/" + std::to_string(note->duration);
    return (result);
}

float ParsedFile::getTotalDuration(const t_notes* notes) {
    float total = 0.0f;
    const t_notes* current = notes;
    while (current != nullptr) {
        total += current->duration;
        current = current->next;
    }
    return (total);
}

int ParsedFile::countNotes(const t_notes* notes) {
    int count = 0;
    const t_notes* current = notes;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return (count);
}

t_notes* ParsedFile::findNoteByPosition(const t_notes* notes, float startTime) {
    float currentTime = 0.0f;
    t_notes* current = const_cast<t_notes*>(notes);
    
    while (current != nullptr) {
        if (currentTime <= startTime) {
            if (startTime < currentTime + current->duration)
                return (current);
        }
        currentTime += current->duration;
        current = current->next;
    }
    return (nullptr);
}

void ParsedFile::trim(std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start != std::string::npos)
        str = str.substr(start);
    
    size_t end = str.find_last_not_of(" \t\n\r");
    if (end != std::string::npos)
        str = str.substr(0, end + 1);

}

int ParsedFile::isCommentLine(const std::string& line) const {
    if (line.empty())
        return (0);
    if (line[0] == '#')
        return (1);
    return (0);
}

int ParsedFile::parseTempoLine(const std::string& line) {
    if (line.find("tempo ") != 0)
        return (0);
    
    std::string numberStr = line.substr(6);
    int temp;
    if (safeStringToInt(numberStr, temp) == 1) {
        tempo = temp;
        return (1);
    }
    return (0);
}

int ParsedFile::parseTracksLine(const std::string& line) {
    if (line.find("tracks ") != 0)
        return (0);
    
    std::string instrumentsStr = line.substr(7);
    std::istringstream stream(instrumentsStr);
    std::string instrument;
    
    int trackNumber = 1;
    while (std::getline(stream, instrument, ',')) {
        trim(instrument);
        t_track newTrack;
        newTrack.trackNumber = trackNumber;
        trackNumber++;
        newTrack.instrument = instrument;
        newTrack.notes = nullptr;
        tracks.push_back(newTrack);
    }
    
    if (tracks.empty())
        return (0);
    return (1);
}

int ParsedFile::parseTrackLine(const std::string& line) {
    size_t colonPos = line.find(':');
    if (colonPos == std::string::npos)
        return (0);

    std::string trackStr = line.substr(0, colonPos);
    std::string notesStr = line.substr(colonPos + 1);

    int trackNumber;
    if (safeStringToInt(trackStr, trackNumber) == 0)
        return (0);
    if (trackNumber < 1)
        return (0);
    if (trackNumber > (int)tracks.size())
        return (0);

    resetLineState();
    parseNotesForTrack(trackNumber - 1, notesStr);
    return (1);
}

void ParsedFile::parseNotesForTrack(int trackIndex, const std::string& notesStr) {
    std::istringstream stream(notesStr);
    std::string token;
    
    while (stream >> token) {
        t_notes note;
        if (parseNoteToken(token, note) == 1)
            addNoteToTrack(trackIndex, note);
    }
}

int ParsedFile::parseNoteToken(const std::string& token, t_notes& note) {
    if (token.find("r/") == 0) {
        float duration;
        if (safeStringToFloat(token.substr(2), duration) == 1) {
            note.pitch = 'r';
            note.alter = '\0';
            note.octave = 0;
            note.duration = duration;
            note.next = nullptr;
            currentDuration = duration;
            return (1);
        }
        return (0);
    }

    // find duration
    size_t slashPos = token.find('/');
    std::string pitchPart;
    std::string durationPart;
    
    if (slashPos != std::string::npos) {
        pitchPart = token.substr(0, slashPos);
        durationPart = token.substr(slashPos + 1);
    } else {
        pitchPart = token;
        durationPart = "";
    }
    float duration = currentDuration;
    if (durationPart.empty() == 0) {
        if (safeStringToFloat(durationPart, duration) == 0)
            return (0);
        currentDuration = duration;
    }
    if (pitchPart.empty())
        return (0);
    
    char pitch = tolower(pitchPart[0]);
    if (pitch < 'a')
        return (0);
    if (pitch > 'g')
        return (0);

    char alteration = '\0';
    int octave = currentOctave;
    size_t pos = 1;

    if (pos < pitchPart.size()) {
        if (pitchPart[pos] == '#' || pitchPart[pos] == 'b') {
            alteration = pitchPart[pos];
            pos++;
        }
    }

    if (pos < pitchPart.size()) {
        int oct;
        if (safeStringToInt(pitchPart.substr(pos), oct) == 1) {
            octave = oct;
            currentOctave = octave;
        }
    }

    note.pitch = pitch;
    note.alter = alteration;
    note.octave = octave;
    note.duration = duration;
    note.next = nullptr;
    
    return (1);
}

void ParsedFile::addNoteToTrack(int trackIndex, const t_notes& note) {
    t_notes* newNote = new t_notes;
    newNote->pitch = note.pitch;
    newNote->alter = note.alter;
    newNote->octave = note.octave;
    newNote->duration = note.duration;
    newNote->next = nullptr;

    if (tracks[trackIndex].notes == nullptr) {
        tracks[trackIndex].notes = newNote;
    } else {
        t_notes* current = tracks[trackIndex].notes;
        while (current->next != nullptr)
            current = current->next;
        current->next = newNote;
    }
}

void ParsedFile::resetLineState() {
    currentOctave = 4;
    currentDuration = 1.0f;
}