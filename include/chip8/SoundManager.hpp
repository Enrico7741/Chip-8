//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_SOUNDMANAGER_HPP
#define CHIP8_SOUNDMANAGER_HPP

#include <SDL.h>
#include <cstdint>

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    // Sound manager is unique in emulator
    SoundManager(const SoundManager &) = delete;
    SoundManager& operator=(const SoundManager &) = delete;
    SoundManager(SoundManager &&) = delete;
    SoundManager& operator=(SoundManager &&) = delete;

    void playSound(bool play);

private:
    SDL_AudioDeviceID audioDevice{0};
    static const int kAmplitude{10000};
    static const int kSampleRate{44100};
    static int sampleNr;

    bool initialize();
    static void audioCallback(void *userdata, uint8_t *audioBuffer, int bufferSize);
};

#endif