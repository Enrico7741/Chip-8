//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/Game.hpp"

int Game::getBestSpeed()
{
    if (bestSpeed.find(name) == bestSpeed.end())
    {
        return defaultSpeed;
    }
    else
    {
        return bestSpeed.at(name);
    }
}