#pragma once

#include <boost/circular_buffer.hpp>
#include <vector>

#include "portaudio.h"

namespace showtime {

class AudioDevice {
public:
    static AudioDevice* getInstance() {
        static AudioDevice instance;
        return &instance;
    }

    static std::vector<float> process();

    static void initialize();
    static void terminate();

private:
    static PaStream* stream;

    AudioDevice() = default;
    virtual ~AudioDevice() = default;

    static int streamCallback(const void* input,
                              void* output,
                              unsigned long frameCount,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void* userData);

    static boost::circular_buffer<float> buffer;
};

} // namespace showtime