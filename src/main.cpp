#include "../inc/parsing.hpp"


void analyzeMusic(const ParsedFile& parser) {
    std::cout << "\n(check for access )" << std::endl;
    
    std::cout << "Tempo: " << parser.getTempo() << " BPM" << std::endl;
    
    const std::vector<t_track>& tracks = parser.getTracks();
    std::cout << "Total tracks: " << tracks.size() << std::endl;
    
    for (size_t i = 0; i < tracks.size(); i++) {
        const t_track& track = tracks[i];
        std::cout << "\n Track " << track.trackNumber << " (" << track.instrument << ")" << std::endl;
        
        int noteCount = ParsedFile::countNotes(track.notes);
        std::cout << "Note count: " << noteCount << std::endl;

        float totalDuration = ParsedFile::getTotalDuration(track.notes);
        std::cout << "Total duration: " << totalDuration << std::endl;

        const t_notes* current = track.notes;
        int count = 0;
        std::cout << "First few notes: ";
        while (current != nullptr && count < 5) {
            std::cout << ParsedFile::noteToString(current) << " ";
            current = current->next;
            count++;
        }
        if (noteCount > 5) {
            std::cout << "...";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Error: Please provide a filename" << std::endl;
        return (1);
    }

    std::string filename = argv[1];
    ParsedFile parser;
    
    if (parser.parseFile(filename) == 1) {
        std::cout << "File parsed successfully!" << std::endl;
        parser.printSummary();
        analyzeMusic(parser);
        return (0);
    } else
        return (1);
}