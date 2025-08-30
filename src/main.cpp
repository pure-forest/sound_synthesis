#include "../inc/parsing.hpp"
#include "../inc/SoundMaking.hpp"

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        std::cout << "Error: Please provide a filename" << std::endl;
        return (1);
    }
    std::string filename = argv[1];
    ParsedFile parser;
	
    if (parser.parseFile(filename) == 1) {
        std::cout << "File parsed successfully!" << std::endl;
		//parser.printSummary();
		SoundMaking test(parser);
		test.printTrack();
    	std::cout << "Done." << std::endl;
        return (0);
    } else
        return (1);
    
//    Track leftHand  = { {{261.63, 1.0}, {293.66, 1.0}}, 0 }; 
//    Track rightHand = { {{440.0, 1.5}, {493.88, 0.5}}, 0 }; 
//    test.makeSoundStereo(leftHand, rightHand);
   // return 0;
}
