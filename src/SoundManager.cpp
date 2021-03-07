//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/SoundManager.hpp"

#include <iostream>

int SoundManager::sampleNr{0};

SoundManager::SoundManager()
{
    initialize();
}

SoundManager::~SoundManager()
{
    SDL_CloseAudioDevice(audioDevice);
}

void SoundManager::playSound(bool play)
{
    SDL_PauseAudioDevice(audioDevice, !play);
}

bool SoundManager::initialize()
{
    SDL_AudioSpec desiredSpec;
    desiredSpec.freq = kSampleRate;       // Samples per second
    desiredSpec.format = AUDIO_S16SYS;    // Sample type: signed 16 bit
    desiredSpec.channels = 1;             // Only one channel
    desiredSpec.samples = 2048;           // Buffer size
    desiredSpec.callback = audioCallback; // Buffer refill callback
    desiredSpec.userdata = &sampleNr;     // Keeping track of current sample number

    SDL_AudioSpec obtainedSpec;
    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &desiredSpec, &obtainedSpec, 0);
    if (audioDevice == 0)
    {
        std::cout << "Error: SDL_OpenAudioDevice: " << SDL_GetError() << std::endl;
        return false;
    }

    if (desiredSpec.format != obtainedSpec.format)
    {
        std::cout << "Error: Couldn't get desired SDL_AudioSpec." << std::endl;
        return false;
    }

    return true;
}

void SoundManager::audioCallback([[maybe_unused]] void *userdata, uint8_t *audioBuffer, int bufferSize)
{
    // note: Userdata reference exists in class, no need to use the parameter itself
    auto length = bufferSize / 2; // 2 bytes per sample for AUDIO_S16SYS
    int16_t *buffer = reinterpret_cast<int16_t *>(audioBuffer);

    for (int i = 0; i < length; i++, sampleNr++)
    {
        auto time = static_cast<double>(sampleNr) / static_cast<double>(kSampleRate); // time in seconds
        buffer[i] = static_cast<int16_t>((kAmplitude * sin(2.0f * M_PI * 441.0f * time))); // Render 441 HZ sine wave
    }
}
