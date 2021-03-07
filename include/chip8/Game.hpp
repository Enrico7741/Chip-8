//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_GAME_HPP
#define CHIP8_GAME_HPP

#include <map>
#include <string>

class Game
{
public:
    Game(const std::string &name, const std::string &path, uintmax_t size) : name{name}, path{path}, size{size} {};
    int getBestSpeed();

    std::string name;
    std::string path;
    uintmax_t size;

private:
    const int defaultSpeed{500};
    const std::map<std::string, int> bestSpeed{{"Brix.ch8", 400},
                                               {"Breakout.ch8", 400},
                                               {"Cave.ch8", 300},
                                               {"Cavern.ch8", 2000},
                                               {"Chip8Picture.ch8", 500},
                                               {"Clock.ch8", 1000},
                                               {"DelayTimerTest.ch8", 300},
                                               {"HearthMonitor.ch8", 700},
                                               {"IBMLogo.ch8", 500},
                                               {"Maze.ch8", 500},
                                               {"MorseCode.ch8", 300},
                                               {"OpcodeTest.ch8", 500},
                                               {"OpcodeTest2.ch8", 500},
                                               {"Particle.ch8", 1500},
                                               {"Pong.ch8", 400},
                                               {"Pong2.ch8", 400},
                                               {"Puzzel.ch8", 1000},
                                               {"RandomTest.ch8", 100},
                                               {"Tetris.ch8", 400},
                                               {"Trip8.ch8", 1000},
                                               {"Ufo.ch8", 400},
                                               {"Wall.ch8", 300},
                                               {"Zero.ch8", 1000}};
};

#endif