#include "../inc/parsing.hpp"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <cmath>

const double PI = 3.14159265358979323846;
const int SAMPLE_RATE = 44100;
const double FREQUENCY = 261.63; // c4
const double DURATION = 1.0;    // seconds
const double AMPLITUDE = 0.5;
const int	BUFFER_SIZE= 256;

static float triangleWave(double frequency, int sampleIndex, int sampleRate) {
    double t = static_cast<double>(sampleIndex) / sampleRate;
    double value = 2.0 * fabs(2.0 * (frequency * t - floor(frequency * t + 0.5))) - 1.0;
    return static_cast<float>(value);
}

static void	makeSound(void)
{
	pa_sample_spec ss;
    ss.format = PA_SAMPLE_FLOAT32LE;
    ss.rate = SAMPLE_RATE;
    ss.channels = 1;

    int error;
    pa_simple *pa = pa_simple_new(nullptr, "SineWaveSynth", PA_STREAM_PLAYBACK, nullptr, "playback", &ss, nullptr, nullptr, &error);

    if (!pa) {
        std::cerr << "PulseAudio error: " << pa_strerror(error) << std::endl;
        exit(1);
    }

    // Generate and play samples
   int totalSamples = SAMPLE_RATE * DURATION;
   float	buffer[BUFFER_SIZE];
	for (int i = 0; i < totalSamples; i += BUFFER_SIZE) {
        int blockSize = std::min(BUFFER_SIZE, totalSamples - i);
        for (int j = 0; j < blockSize; ++j) {
            int sampleIndex = i + j;
            buffer[j] = AMPLITUDE * triangleWave(FREQUENCY, sampleIndex, SAMPLE_RATE);
        }

        // Write block to PulseAudio
        if (pa_simple_write(pa, buffer, blockSize * sizeof(float), &error) < 0) {
            std::cerr << "PulseAudio write error: " << pa_strerror(error) << std::endl;
            break;
        }
	}
    // Drain and clean up
    pa_simple_drain(pa, &error);
    pa_simple_free(pa);

    std::cout << "Done playing tone." << std::endl;
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
    	std::cout << "Done." << std::endl;
		makeSound();
        return (0);
    } else
        return (1);
}
