//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_USERINTERFACE_HPP
#define CHIP8_USERINTERFACE_HPP

#include "chip8/Chip8.hpp"
#include "chip8/SoundManager.hpp"
#include "chip8/MemoryDumper.hpp"
#include "chip8/RenderManager.hpp"
#include "chip8/sections/ISection.hpp"

#include <SDL.h>
#include <memory>
#include <vector>
#include <cstdint>

class UserInterface
{
public:
    UserInterface(Chip8 &chip8) : chip8{chip8} {};
    ~UserInterface();
    bool initialize();
    void run();

private:
    Chip8 &chip8;
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool warpMode{false};

    std::unique_ptr<SoundManager> soundManager{};
    std::unique_ptr<MemoryDumper> memoryDumper{};
    std::shared_ptr<RenderManager> renderManager{};
    std::vector<std::unique_ptr<ISection>> sections{};

    SDL_TimerID redrawTimerId;
    SDL_TimerID emulatorTimerId;

    static const int kRedrawEvent{0};
    static const int kCatchUpEvent{1};
    static const int kRedrawInterval{17};
    static const int kCatchUpInterval{2};

    bool initializeWindow();
    void startRedrawTimer();
    void startEmulatorTimer();
    void stopTimer(SDL_TimerID timerId);
    void handleTimerEvent(SDL_Event &event, const Chip8State &state);
    void handleInputEvent(SDL_Event &event, const Chip8State &state);
    void handleDropEvent(SDL_Event &event);
    void startWarpMode();
    void stopWarpMode();
    void warp();
    static uint32_t timerCallback(uint32_t interval, void *param);
    void updateScreen();
};

#endif