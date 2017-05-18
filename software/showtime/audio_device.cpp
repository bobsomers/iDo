#include "audio_device.h"

#include <iostream>
#include <cmath>
#include <mutex>

#include "fftw3.h"

constexpr unsigned int kSampleRate = 8000;
constexpr unsigned int kBufferSize = kSampleRate / 10;
constexpr float PI = 3.1415927;

std::mutex mtx;

namespace showtime {

PaStream* AudioDevice::stream = nullptr;

boost::circular_buffer<float> AudioDevice::buffer(kBufferSize);

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
                               1, // Mono input
                               0, // No output channel
                               paFloat32,
                               kSampleRate,
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

void lowPassFilter(std::vector<float>& data) {
    static float lastInput = 0;
    for (float& input : data) {
        input = ( input + lastInput ) / 2 ;
    }
}

void applyWindow(std::vector<float>& data) {
    const auto size = data.size();
    int i = 0;
    for (float& input : data) {
        // Build Hann window component
        const auto window = 0.5 * ( 1 - cos( 2 * PI * i / (size-1.0) ) );

        // Apply window
        input = input * window;
        ++i;
    }
}

std::vector<float> AudioDevice::process() {
    if (buffer.size() != 800) {
        return {};
    }

    std::lock_guard<std::mutex> lock(mtx);

    // Pull data from circular buffer
    std::vector<float> data;
    data.insert(data.begin(), AudioDevice::buffer.begin(), AudioDevice::buffer.end());

    // Low pass filter to reduce noise
    lowPassFilter(data);

    // Apply Hann window to reduce sidelobes
    applyWindow(data);

    // unsigned int N = data.size();

    // // Apply FFT
    // float* out = (float*)fftwf_malloc(sizeof(float) * N);
    // fftwf_plan p;
    // p = fftwf_plan_r2r_1d(N, &data.front(), out, FFTW_R2HC, FFTW_FORWARD);
    // fftwf_execute(p);

    // std::vector<float> output(N/2 + 1);

    // output[0] = out[0]*out[0];  /* DC component */
    // for (unsigned int k = 1; k < (N+1)/2; ++k)  /* (k < N/2 rounded up) */
    //     output[k] = out[k]*out[k] + out[N-k]*out[N-k];
    // if (N % 2 == 0) /* N is even */
    //     output[N/2] = out[N/2]*out[N/2];  /* Nyquist freq. */

    // fftwf_free(out);

    return output;
}

int AudioDevice::streamCallback(const void* input,
                                void* output,
                                unsigned long frameCount,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void* userData) {
    std::lock_guard<std::mutex> lock(mtx);

    // Do all the things
    float* in = (float*)input;
    for (unsigned long i = 0; i < frameCount; ++i) {
        AudioDevice::buffer.push_back(*(in++));
    }

    return 0;
}

} // namespace showtime