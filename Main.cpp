//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/Chip8.hpp"
#include "chip8/UserInterface.hpp"

#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Error: Game file not specified" << std::endl;
        return EXIT_FAILURE;
    }

    Chip8 chip8;
    std::string gamePath(argv[1]);
    if (!chip8.loadGame(gamePath))
    {
        std::cout << "Error: Couldn't load game file" << std::endl;
        return EXIT_FAILURE;
    }
    
    UserInterface userInterface(chip8);
    if (!userInterface.initialize())
    {
        std::cout << "Error: UserInterface initialization failed" << std::endl;
        return EXIT_FAILURE;
    }
    userInterface.run();

    return EXIT_SUCCESS;
}