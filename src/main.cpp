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
		// test.printTrack();
		test.makeSound();
    	std::cout << "Done." << std::endl;
        return (0);
    } else
        return (1);


}
