//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/Layout.hpp"
#include "chip8/Buttons.hpp"
#include "chip8/UserInterface.hpp"
#include "chip8/sections/InfoSection.hpp"
#include "chip8/sections/InputSection.hpp"
#include "chip8/sections/BreakpointSection.hpp"
#include "chip8/sections/StackSection.hpp"
#include "chip8/sections/RegisterSection.hpp"
#include "chip8/sections/CodeSection.hpp"
#include "chip8/sections/DisplaySection.hpp"

#include <string>
#include <iostream>
#include <algorithm>

UserInterface::~UserInterface()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

bool UserInterface::initialize()
{
    if (!initializeWindow())
    {
        return false;
    }

    soundManager = std::make_unique<SoundManager>();
    memoryDumper = std::make_unique<MemoryDumper>();
    renderManager = std::make_unique<RenderManager>(renderer);

    // Add all desired sections to the section list
    sections.emplace_back(std::make_unique<InfoSection>(renderManager));
    sections.emplace_back(std::make_unique<InputSection>(renderManager));
    sections.emplace_back(std::make_unique<BreakpointSection>(renderManager));
    sections.emplace_back(std::make_unique<StackSection>(renderManager));
    sections.emplace_back(std::make_unique<RegisterSection>(renderManager));
    sections.emplace_back(std::make_unique<DisplaySection>(renderManager));
    sections.emplace_back(std::make_unique<CodeSection>(renderManager));

    return true;
}

bool UserInterface::initializeWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        std::cout << "Error: SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create our main window
    window = SDL_CreateWindow("Chip-8 Emulator",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              Layout::MainWindow::width, Layout::MainWindow::height,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cout << "Error: SDL_CreateWindow: " << SDL_GetError() << std::endl;
        return false;
    }

    // Try to load the chip-8 taskbar logo
    auto icon = SDL_LoadBMP("data/bitmaps/windowIcon.bmp");
    if (icon == nullptr)
    {
        std::cout << "Error: SDL_LoadBMP: " << SDL_GetError() << std::endl;
    }
    else
    {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    // Create a renderer for the window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cout << "Error: SDL_CreateRenderer: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void UserInterface::run()
{
    chip8.start();
    startRedrawTimer();
    startEmulatorTimer();

    SDL_Event event;
    auto &state = chip8.getState();
    auto running = true;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_USEREVENT)
            {
                handleTimerEvent(event, state);
            }
            else if (event.type == SDL_KEYDOWN | event.type == SDL_KEYUP)
            {
                handleInputEvent(event, state);
            }
            else if (event.type == SDL_DROPFILE)
            {
                handleDropEvent(event);
            }
            else if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
    }
}

void UserInterface::startRedrawTimer()
{
    redrawTimerId = SDL_AddTimer(kRedrawInterval, timerCallback, nullptr);
}

void UserInterface::startEmulatorTimer()
{
    emulatorTimerId = SDL_AddTimer(kCatchUpInterval, timerCallback, nullptr);
}

void UserInterface::stopTimer(SDL_TimerID timerId)
{
    SDL_RemoveTimer(timerId);
}

void UserInterface::handleTimerEvent(SDL_Event &event, const Chip8State &state)
{
    if (event.user.code == kRedrawEvent)
    {
        chip8.updateTimers();
        soundManager->playSound(state.soundTimer > 0);
        updateScreen();

        /**
         * If we are in warp mode we need to warp between the draw calls.
         * If not we can sleep for a bit so the cpu isn't working all the time.
         */
        if (warpMode)
        {
            warp();
        }
        else
        {
            SDL_Delay(10);
        }
    }
    else if (event.user.code == kCatchUpEvent && state.isRunning)
    {
        chip8.catchUp();
    }
}

void UserInterface::handleInputEvent(SDL_Event &event, const Chip8State &state)
{
    /**
     * Here we handle user input. It's a bit messy but I haven't figured
     * out a nicer way of doing it yet.
     */
    const auto &key = event.key.keysym.sym;
    const auto pressed = (event.type == SDL_KEYDOWN);

    if (key == SDLK_F1 && pressed)
    {
        if (state.isRunning)
        {
            chip8.stop();
            if (warpMode)
            {
                stopWarpMode();
            }
        }
        else
        {
            chip8.start();
        }
    }
    else if (key == SDLK_F2 && pressed && !state.isRunning)
    {
        chip8.emulateCycle();
    }
    else if (key == SDLK_F3 && pressed)
    {
        chip8.toggleBreakpoint();
    }
    else if (key == SDLK_F4 && pressed)
    {
        if (warpMode)
        {
            stopWarpMode();
        }
        else
        {
            startWarpMode();
        }
    }
    else if (key == SDLK_F5 && pressed)
    {
        memoryDumper->dumpMemory(state.memory);
    }
    else if (key == SDLK_F6 && pressed)
    {
        if (chip8.loadGame(state.game->path) && warpMode)
        {
            stopWarpMode();
        }
    }
    else if (key == SDLK_PLUS && pressed)
    {
        chip8.increaseSpeed();
    }
    else if (key == SDLK_MINUS && pressed)
    {
        chip8.decreaseSpeed();
    }
    else
    {
        // Searching the key in our chip-8 keymap
        auto index = std::find_if(keyMap.begin(), keyMap.end(),
                                  [&](const auto &x) { return x.first == key; });

        // If the key is in the keymap we inform the chip-8
        if (index != keyMap.end())
        {
            chip8.setButton(pressed, index->second);
        }
    }
}

void UserInterface::handleDropEvent(SDL_Event &event)
{
    std::string path(event.drop.file);
    if (chip8.loadGame(path) && warpMode)
    {
        stopWarpMode();
    }
    SDL_free(event.drop.file);
}

void UserInterface::startWarpMode()
{
    warpMode = true;
    chip8.start();
    stopTimer(emulatorTimerId);
}

void UserInterface::stopWarpMode()
{
    warpMode = false;
    startEmulatorTimer();
}

void UserInterface::warp()
{
    static auto counter = 0;
    static auto executedInstructions = 0;

    executedInstructions += chip8.executeMs(16);
    counter++;

    // After looping 62 times for 16 ms we have 1 second and can print the result
    if (counter >= 62)
    {
        std::cout << "Instructions per second: " << executedInstructions << std::endl;
        counter = 0;
        executedInstructions = 0;
    }
}

uint32_t UserInterface::timerCallback(uint32_t interval, void *param)
{
    /**
     * Every time a timer calls this methode an event gets pushed
     * to the event queue of the program so we can handle it.
     */
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = (interval == kRedrawInterval) ? kRedrawEvent : kCatchUpEvent;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return (interval);
}

void UserInterface::updateScreen()
{
    for (const auto &section : sections)
    {
        section->redraw(chip8.getState());
    }
    renderManager->updateScreen();
}