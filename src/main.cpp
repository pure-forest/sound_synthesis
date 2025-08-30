#include "../inc/parsing.hpp"
#include "../inc/SoundMaking.hpp"

void printTrack(const Track& track) {
    std::cout << "Track - Wave Type: " << track.waveType << std::endl;
    std::cout << "Notes:" << std::endl;
    
    for (size_t i = 0; i < track.notes.size(); i++) {
        const Note& note = track.notes[i];
        std::cout << "  " << i + 1 << ". Frequency: " << note.frequency << " Hz";
        std::cout << ", Duration: " << note.duration << " seconds";
        
        if (note.frequency == 0.0) {
            std::cout << " (REST)";
        }
        
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        std::cout << "Error: Please provide a filename" << std::endl;
        return (1);
    }
    std::string filename = argv[1];
    ParsedFile parser;
	SoundMaking test;
    if (parser.parseFile(filename) == 1) {
        std::cout << "File parsed successfully!" << std::endl;
		//parser.printSummary();
		for (int i = 0; i < parser.getTrackCount(); i++){
			//int index = parser.getTrack(i);
			Track soundTrack = test.ParsedToSound(*parser.getTrack(i), parser.getTempo(), parser.getWaveType(i));
			printTrack(soundTrack);
		}
    	std::cout << "Done." << std::endl;
        return (0);
    } else
        return (1);
    
//    Track leftHand  = { {{261.63, 1.0}, {293.66, 1.0}}, 0 }; 
//    Track rightHand = { {{440.0, 1.5}, {493.88, 0.5}}, 0 }; 
//    test.makeSoundStereo(leftHand, rightHand);
   // return 0;
}
