#include "audio_device.h"

#include <iostream>

#define SAMPLE_RATE (44100)

namespace showtime {

PaStream* AudioDevice::stream = nullptr;

void AudioDevice::initialize() {
    // Don't allow multiple initializations
    if (AudioDevice::stream) {
        std::cout << "Error: Audio stream already initialized.";
        std::exit(EXIT_FAILURE);
    }

    // Initialize portaudio
    auto err = Pa_Initialize();

    if (err != paNoError) {
        std::cout << "Failed to initialize portaudio." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Open up a default stream (stereo output)
    err = Pa_OpenDefaultStream(&stream,
                               2, // Stereo input
                               0, // No output channel
                               paFloat32,
                               SAMPLE_RATE,
                               paFramesPerBufferUnspecified,
                               &AudioDevice::streamCallback,
                               NULL);

    if (err != paNoError) {
        std::cout << "Failed to open audio stream." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Attempt to start the stream
    err = Pa_StartStream(AudioDevice::stream);

    if (err != paNoError) {
        std::cout << "Failed to start audio stream." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Audio device initialized" << std::endl;
}

void AudioDevice::terminate() {
    // Stop and destroy the audio stream 
    auto err = Pa_StopStream(AudioDevice::stream);
    AudioDevice::stream = nullptr;

    if (err != paNoError) {
        std::cout << "Failed to stop audio stream." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Terminate portaudio
    err = Pa_Terminate();
    if (err != paNoError) {
        std::cout << "Failed to terminate portaudio." << std::endl;
        std::exit(EXIT_FAILURE);
    }

}

int AudioDevice::streamCallback(const void* input,
                                void* output,
                                unsigned long frameCount,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void* userData) {
    // Do all the things
    return 0;
}

} // namespace showtime