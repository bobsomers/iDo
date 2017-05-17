#pragma once

#include "portaudio.h"

namespace showtime {

class AudioDevice {
public:
    static AudioDevice* getInstance() {
        static AudioDevice instance;
        return &instance;
    }

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

};

} // namespace showtime