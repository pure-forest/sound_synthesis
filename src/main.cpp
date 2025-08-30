#include "../inc/parsing.hpp"
#include "../inc/SoundMaking.hpp"

int main(int argc, char* argv[])
{
    ParsedFile parser;

    if (argc != 2)
    {
        std::cerr << "Error: Please provide a filename" << std::endl;
        return (1);
    }
    std::string filename = argv[1];
    if (parser.parseFile(filename) != 1) 
    {
        std::cerr << "File parsed failed!" << std::endl;
        return (1);
    }
    SoundMaking synth(parser);
    synth.makeSound();
    return (0);
}
